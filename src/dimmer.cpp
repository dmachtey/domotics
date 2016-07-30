// Filename: dimmer.cpp
//
// Description: implement the DIMMER class. aims to control a light intensity
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 15:11:07 2016 (-0500)
//
// Last-Updated: Fri Jul 29 21:40:03 2016 (-0500)
//           By: Damian Machtey
//     Update #: 67

// Change Log:
//
//
// Copyright (C) 2016 Damian Machtey
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//

// Code:

#include <cstring>
#include <iostream>
#include "dimmer.h"

namespace lighting{

  DIMMER::DIMMER(const char* id, const char* host, int port, const char* name,
                  uint auto_off_time, double power, int max_level) :
    COIL (id, host, port, name, auto_off_time, power){
    this->max_level = max_level;
  }


  uint DIMMER::looop(unsigned int scan_time, bool sw, bool door_bell_sw){
    bool longpress = false;
    bool shortpress = false;
    uint next_slot = 0;

    longpress = (sw_press_acc > MIN_SCALING_TIME);
    shortpress = ((sw_press_acc > 0) && !sw && !longpress);

    sw_press_acc += scan_time; //how long is the sw pressed.
    sw_press_acc *= (sw==true);

    ringing_acc += scan_time;
    ringing_acc *= ((ringing||ringing_latch)==true);

    // timeout, turn it off
    time_off_acc += scan_time * (time_off_sp>0);
    time_off_acc *=(on==true);
    if(time_off_acc > time_off_sp) going_off = true;

    // auto publish
    republish_acc += scan_time;
    if(republish_acc > REPUBLISH_TIME){
      publish_now();
      republish_acc = 0;
    }

    going_on_off_acc += scan_time; //Steps at automatic fadding

    next_slot = (sw_press_acc - MIN_SCALING_TIME)/SCALING_TIME; //for scaling on long press

    if (shortpress && ((going_on || going_off))){
      going_on = false; //reset
      going_off = false; //reset
      shortpress = false;
    }

    if (shortpress && !on) going_on = true; //set
    if (shortpress && on) going_off = true; //set


    if (going_on_off_acc > 12){
    if (going_on) goingOn();
    if (going_off) goingOff();
    going_on_off_acc = 0;
    }

    if (!sw){
      sw_slots = 0; //reset the counter
      up_down = false; //go down always first
      longpress = false;
    }

    if (longpress && (sw_slots < next_slot)) //fade
      fading();

    if(COIL::loop(15,1)){
      reconnect();
    }

    // Ring the light
    if (door_bell_sw || ringing_latch){
      duty = (((ringing_acc / 350) % 2) == 0)*70+15;
      ringing = true;
      ringing_latch = true;
      if (ringing_acc > 3000) ringing_latch = false;
      std::cout <<  "ringing at: " << duty << std::endl;
    }
    if (ringing && !ringing_latch) { // go back to the start value
      duty = old_duty;
      ringing = false;
    }

    if (((duty != old_duty) || (republish_acc > REPUBLISH_TIME)) && !going_on && !going_off && !ringing_latch){
      old_duty = duty;
      republish_acc *= (republish_acc < REPUBLISH_TIME); //Reset the counter when published was forced
      publish_now();
    }

    if (duty >= max_level) duty = max_level;
    if (duty <= 0) duty = 0;

    on = (duty > 0);

    return duty;
  }


  void DIMMER::swOffNow(){
    duty = 0;
    publish_now();
  }


  void DIMMER::swOnNow(){
    duty = max_level;
    publish_now();
  }


  void DIMMER::goOn(){
    going_off = false;
    on = going_on = true;
  }


  void DIMMER::goOff(){
    going_off = true;
    going_on = false;
  }


  void DIMMER::goingOff(){
   duty -= (6*max_level)/100;
    if (duty <= ((5*max_level)/100)){
      duty = 0;
      going_off = 0;
    }
    std::cout << "goingOff, duty at: " << duty << std::endl;
  }


  void DIMMER::goingOn(){
   duty += (6*max_level)/100;
   if (duty >= max_level){
     duty = max_level;
     going_on = 0;
   }
  }


  void DIMMER::fading(){
       sw_slots += 1;
    if (up_down){ //up
      duty += max_level/DIMMING_STEPS;
      if (duty >= max_level){
        duty = max_level;
        up_down = 0;
      }
    } // up
    else{ // down
      duty -= max_level/DIMMING_STEPS;
      if (duty <= 0){
        duty = max_level/DIMMING_STEPS;
        up_down = 1;
      }
    } // down
  }


    void DIMMER::on_connect(int rc){
      std::cout << "Connected with code: " << rc << " from DIMMER::" << mqtt_name << std::endl;
      if(rc == 0){
        /* Only attempt to subscribe on a successful connect. */
        std::string sub = mqtt_name + "/get/#";
        subscribe(NULL, sub.c_str());
    }
  }


  void DIMMER::publish_now(){
    std::string topic = mqtt_name + "/send/duty";
    std::string payload = std::to_string(duty);
    publish(NULL, topic.c_str(), payload.length() , payload.c_str());

    std::cout << mqtt_name << " is now at: " << duty << std::endl;
    std::cout << "payload: "  << payload << std::endl;
    std::cout << "topic: " << topic << std::endl;
  }


  void DIMMER::on_message(const struct mosquitto_message *message){
    std::string search_msg = mqtt_name + "/get/duty";
    if(!strcmp(message->topic, search_msg.c_str())){
      duty = atof((char *)message->payload);
      time_off_acc = 0;
    }
    search_msg = mqtt_name + "/get/ringing";
    if(!strcmp(message->topic, search_msg.c_str())){
      ringing_latch = true;
    }
  }

}//namespace
//
// DIMMER.cpp ends here

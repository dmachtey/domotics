// Filename: dimmer.cpp
//
// Description: implement the DIMMER class. aims to control a light intensity
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 15:11:07 2016 (-0500)
//
// Last-Updated: Fri Jul 29 18:32:35 2016 (-0500)
//           By: Damian Machtey
//     Update #: 29

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

#include "dimmer.h"

namespace lighting{

  DIMMER::DIMMER(const char* id, const char* host, int port, const char* name,
                  uint auto_off_time, double power, int max_level) :
    COIL (id, host, port, name, auto_off_time, power){
    this->max_level = max_level;
  }


  uint DIMMER::loop(unsigned int scan_time, bool sw, bool door_bell_sw){
    bool longpress = false;
    bool shortpress = false;
    uint next_slot = 0;

    /*******************************************************************/
    longpress = (sw_press_acc > MIN_SCALING_TIME);
    shortpress = ((sw_press_acc > 0) && !sw && !longpress);

    sw_press_acc += scan_time; //how long is the sw pressed.
    sw_press_acc *= (sw==true);

    ringing_acc += scan_time;
    ringing_acc *= ((ringing||ringing_latch)==true);

    // timeout, turn it off
    time_off_acc += scan_time * (time_off_sp>0);
    time_off_acc *=(on==true);
    if(time_off_acc > time_off_sp)
      goingOff();

    // auto publish
    republish_acc += scan_time;
    if(republish_acc > REPUBLISH_TIME){
      publish_now();
      republish_acc = 0;
    }

    going_on_off_acc += scan_time; //Steps at automatic fadding

    next_slot = (sw_press_acc - MIN_SCALING_TIME)/SCALING_TIME; //for scaling on long press

    if (shortpress && ((going_on || going_off))){
      going_on = 0; //reset
      going_off = 0; //reset
      shortpress = 0;
    }

    if (shortpress && !on) going_on = 1; //set
    if (shortpress && on) going_off = 1; //set


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
    }
    if (ringing && !ringing_latch) { // go back to the start value
      duty = old_duty;
      ringing = false;
    }

    if (((duty != old_duty) || republish_acc > REPUBLISH_TIME) && !going_on && !going_off && !ringing_latch){
      old_duty = duty;
      republish_acc *= (republish_acc < REPUBLISH_TIME); //Reset the counter when published was forced
      publish_now();
    }

    if (duty >= max_level) {
      duty = max_level;
      on = true;
        }
    if (duty <= 0) {
      duty = 0;
      on = false;
        }
    /*******************************************************************/
    return duty;
  }


  void DIMMER::goOffNow(){
    duty = 0;
    on = false;
  }


  void DIMMER::goOnNow(){
    duty = max_level;
    on = true;
  }


  void DIMMER::goOn(){
    going_on = true;
    going_off = false;
  }


  void DIMMER::goOff(){
    going_off = true;
    going_on = false;
  }


  void DIMMER::goingOff(){
  }


  void DIMMER::goingOn(){
  }


  void DIMMER::fading(){
  }


}//namespace
//
// DIMMER.cpp ends here

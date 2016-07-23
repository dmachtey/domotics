// Filename: dimmer_ctl.cpp
// 
// Description: 
// Author: Damian Machtey
// Maintainer: 
// 
// Created: 2015-06-22 Mon
// 
// Last-Updated: Sat Jul 23 12:46:08 2016 (-0500)
//           By: Damian Machtey
//     Update #: 3

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


#include "dimmer_ctl.h"
#include <iostream> //cin cout cerr (for errors) clog (for logs)
#include <cstring> // memcpy, strcmp, strlen...

using namespace std;

#define MIN_SCALING_TIME 1000 // ms to start dimming (less shortpress)
#define SCALING_TIME 500 // ms for each dimming step
#define DIMMING_STEPS 20 // number of dimming steps
#define TIME_OUT_PUBLISH 2*60*1000

namespace lighting{
  /*
   * unsigned int positive_edge(unsigned int value, unsigned int *mem){
   *   /\* this function is intended to detect the positive edge change of a
   *      input signal value *\/
   *   int out = (value && !(*mem))? 1 : 0;
   *   *mem = value;
   *   return out;
   * }
   *
   * unsigned int negative_edge(unsigned int value, unsigned int *mem){
   *   /\* this function is intended to detect the negative edge change of a
   *      input signal value *\/
   *   int out = (!value && *mem)? 1 : 0;
   *   *mem = value;
   *   return out;
   * }
   */

  DIMMER::DIMMER(const char *id, const char *host, int port, char *name) : mosquittopp(id){
    duty = 0;
    old_dutyMQ = 0;
    up_down = 0;
    ms = 0;
    ms_g = 0;
    on = 0;
    going_on = 0;
    going_off = 0;
    mqtt_name = name;
    publish_ms = rand()%(TIME_OUT_PUBLISH);
    ringing_latch = 0;
    ringing = 0;
    int keepalive = 60;
    connect(host, port, keepalive);
    publishnow();
  }

  /*
   * | press | is        | start     | on            |
   * |-------+-----------+-----------+---------------|
   * | short | off       | going on  | release       |
   * | short | on        | going_off | release       |
   * | short | going_on  | stop      | release       |
   * | short | going_off | stop      | release       |
   * | long  | off       | fade in   | while pressed |
   * | long  | quiet     | fade out  | while pressed |
   */
  void DIMMER::dloop(unsigned int sw, unsigned int last_loop_ms, unsigned int ring){
    unsigned int shortpress = 0, longpress = 0;
    unsigned int nt_slots = 0;

    longpress = (ms > MIN_SCALING_TIME);
    shortpress = ((ms>0) && !sw && !longpress);

    ms += last_loop_ms; //how long is the sw pressed.
    ms *= (sw>0);

    ms_g += last_loop_ms;
    ms_g *= ((ring+ringing_latch)>0);

    publish_ms += last_loop_ms;

    ms_going_on_off += last_loop_ms;

    nt_slots = (ms - MIN_SCALING_TIME)/SCALING_TIME; //for scaling on long press
    if (shortpress && ((going_on || going_off))){
      going_on = 0; //reset
      going_off = 0; //reset
      shortpress = 0;
    }

    if (shortpress && !on) going_on = 1; //set
    if (shortpress && on) going_off = 1; //set

    //  if (!going_on && !going_off)
    //  old_duty = duty;

    /*
     * if (going_on) goingON(last_loop_ms);
     * if (going_off) goingOFF(last_loop_ms);
     */

    if (ms_going_on_off > 12){
    if (going_on) goingON(150);
    if (going_off) goingOFF(150);
    ms_going_on_off = 0;
    }



    if (!sw){
      time_slots = 0; //reset the counter
      up_down = 0; //go down always first
      longpress = 0;
    }

    if (longpress && (time_slots < nt_slots)) //fade
      fadeOUT();

    if(loop(15,1)){
      reconnect();
    }

    // Ring the light
    if (ring || ringing_latch){
      duty = (((ms_g / 350) % 2) == 0)*70+15;
      ringing = 1;

      ringing_latch = 1;
      if (ms_g > 3000) ringing_latch = 0;
    }
    if (ringing && !ringing_latch) { // go back to the start value
      duty = old_dutyMQ;
      ringing = 0;
    }

    if (((duty != old_dutyMQ) || publish_ms > TIME_OUT_PUBLISH) && !going_on && !going_off && !ringing_latch){
      old_dutyMQ = duty;
      publish_ms *= (publish_ms < TIME_OUT_PUBLISH); //Reset the counter when published was forced
      publishnow();
    }

    if (duty >= 100) {
      duty = 100;
      on = 1;
        }
    if (duty <= 0) {
      duty = 0;
      on = 0;
        }
  }//loop

  void DIMMER::goingON(unsigned int last_loop_ms){
    //  duty =  duty + (100.0f * ((float)(last_loop_ms)/1000.0f));
    duty += 6;
    on = 1;
    if (duty >= 100){
      duty = 100;
      going_on = 0;
    }
  }

  void DIMMER::goingOFF(unsigned int last_loop_ms){
    // duty =  duty - (100.0f * ((float)(last_loop_ms)/1000.0f));
    duty -= 6;
    if (duty <= 5){
      duty = 0;
      on = 0;
      going_off = 0;
    }
  }

  void DIMMER::fadeOUT(){
    time_slots += 1;
    if (up_down){ //up
      duty += 100.0f/DIMMING_STEPS;
      if (duty >= 100){
        duty = 100;
        up_down = 0;
      }
    } // up
    else{ // down
      duty -= 100.0f/DIMMING_STEPS;
      if (duty <= 0){
        duty = 100.0f/DIMMING_STEPS;
        up_down = 1;
      }
    } // down
    on = duty > 0;
  } // dimming

  void DIMMER::on_connect(int rc)
  {
	cout << "Connected with code: " << rc << " from DIMMER::" << mqtt_name << endl;
	if(rc == 0){
      /* Only attempt to subscribe on a successful connect. */
      string sub = mqtt_name + "/get/#";
      subscribe(NULL, sub.c_str());
	}
  }

  void DIMMER::on_message(const struct mosquitto_message *message)
  {
    string search_msg = mqtt_name + "/get/duty";
    if(!strcmp(message->topic, search_msg.c_str())){
      duty = atof((char *)message->payload);
      on = duty > 0;
    }
  }

  void DIMMER::on_subscribe(int mid, int qos_count, const int *granted_qos)
  {
	cout << "Subscription succeeded from DIMMER::" << mqtt_name << endl;
  }

  void DIMMER::publishnow()
  {
    string topic = mqtt_name + "/send/duty";
    string payload = to_string(duty);  // CFLAGS=-std=c++11 on Makefile
    publish(NULL, topic.c_str(), payload.length() , payload.c_str());
  }

  DIMMER::~DIMMER(){
    cout << "destroying DIMMER::" << mqtt_name << endl;
  }
} /* namespace lighting */

// 
// dimmer_ctl.cpp ends here

// Filename: dimmer_ctl.h
// 
// Description: 
// Author: Damian Machtey
// Maintainer: 
// 
// Created: 2015-06-22 Mon
// 
// Last-Updated: Sat Jul 23 12:42:48 2016 (-0500)
//           By: Damian Machtey
//     Update #: 2

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


#ifndef dimmer_ctl_H_
#define dimmer_ctl_H_
#include <string>

#include <mosquittopp.h>

using std::string;

namespace lighting{

  unsigned int positive_edge(unsigned int value, unsigned int *mem);
  unsigned int negative_edge(unsigned int value, unsigned int *mem);



  class DIMMER : public mosqpp::mosquittopp{
   private:
    unsigned int on; // the light is on
    unsigned int duty; // actual duty
    unsigned int old_dutyMQ;
    unsigned int up_down;  // dimmering up and down
    unsigned int ms; // switch pressed milliseconds
    unsigned int ms_g;
    unsigned int ms_going_on_off; // for going on and off
    unsigned int publish_ms;
    unsigned int time_slots; // to count how many slots is the sw pressed
    unsigned int going_on;
    unsigned int going_off;
    unsigned int ringing;
    unsigned int ringing_latch;
    string mqtt_name;
  public:
    DIMMER(const char *id, const char *host, int port, char *name);
    void dloop(unsigned int sw, unsigned int last_loop_ms, unsigned int ring);
    unsigned int getDuty() {return duty;}
    ~DIMMER();
  private:
    void goingON(unsigned int last_loop_ms);
    void goingOFF(unsigned int last_loop_ms);
    void fadeIN();
    void fadeOUT();
    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void publishnow();
  };
} /* namespace lighting */

#endif /* dimmer_ctl_H_ */

// 
// dimmer_ctl.h ends here

// Filename: coil.cpp
//
// Description: Implement the COIL class
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 12:04:29 2016(-0500)
//
// Last-Updated: Thu Jul 28 21:31:47 2016 (-0500)
//           By: Damian Machtey
//     Update #: 57

// Change Log:
//
//
// Copyright(C) 2016 Damian Machtey
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

#include <iostream> //cin cout cerr(for errors) clog(for logs)
#include <cstring> // memcpy, strcmp, strlen...
#include "coil.h"


namespace lighting{
  COIL::COIL(const char* id, const char* host, int port, const char* name,
             uint auto_off_time, double power) : mosquittopp(id)
  {
    time_off_sp = auto_off_time;
    mqtt_name = name;
    int keepalive = 60;
    connect(host, port, keepalive);
    // publish_now();
    republish_acc = rand()%(REPUBLISH_TIME);
    this->power = power;
  }

  bool COIL::looop(unsigned int scan_time, bool sw)
  {
    if((sw_press_acc==0) && sw){
      if(on) goOff();
      else goOn();
    }

    sw_press_acc += scan_time;
    sw_press_acc *= (sw==true);

    // auto publish
    republish_acc += scan_time;
    if(republish_acc > REPUBLISH_TIME)
      publish_now();

    // timeout, turn it off
    time_off_acc += scan_time * (time_off_sp>0);
    time_off_acc *=(on==true);
    if(time_off_acc > time_off_sp)
      goOff();

    // needed by mosquittopp
    if(loop(15,1)) reconnect();

    actual_power = on ? power : 0.0;
    return on;
  }

  /**
   */
  void COIL::goOn()
  {
    on = true;
    time_off_acc = 0;
    publish_now();
  }

  /**
   */
  void COIL::goOff()
  {
    on = false;
    time_off_acc = 0;
    publish_now();
  }

  void COIL::on_connect(int rc)
  {
    std::cout << "Connected with code: " << rc << " from COIL::" << mqtt_name << std::endl;
    if(rc == 0){
      /* Only attempt to subscribe on a successful connect. */
      std::string sub = mqtt_name + "/get/#";
      subscribe(NULL, sub.c_str());
    }

  }

  void COIL::on_message(const struct  mosquitto_message *message)
  {
    std::string search_msg = mqtt_name + "/get/status";
    if(!strcmp(message->topic, search_msg.c_str())){
      if(atoi((char *)message->payload)>0) goOn();
      else goOff();
    }
  }

  void COIL::on_subscribe(int mid, int qos_count, const int* granted_qos)
  {
    std::cout << "Subscription succeeded from COIL::" << mqtt_name << std::endl;
  }

  void COIL::publish_now()
  {
    std::string topic = mqtt_name + "/send/status" ;
    std::string payload = on ? "on" : "off";  // CFLAGS=-std=c++11 on Makefile
    publish(NULL, topic.c_str(), payload.length() , payload.c_str());

    std::cout << mqtt_name << " is now " << std::boolalpha << on << std::endl;
    std::cout << "payload: "  << payload << std::endl;
    std::cout << "topic: " << topic << std::endl;
  }

  void COIL::compute_power(time_t scan_time)
  {
    std::cout << "to be implemented" << std::endl;
    // total_power
    // total_power_all_clases
  }

} //namespace lighting
//
// COIL.cpp ends here

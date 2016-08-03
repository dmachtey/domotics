// Filename: temperature.cpp
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: 2015-06-22 Mon
//
// Last-Updated: Tue Aug  2 20:20:42 2016 (-0500)
//           By: Damian Machtey
//     Update #: 34

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


#include <iostream>
#include <fstream>
#include <cstring>
#include <errno.h> // for errno

#include "temperature.h"

namespace lighting{

  TEMPERATURE::TEMPERATURE(std::string id, std::string host, int port,
                           std::string where, uint publish_every_ms) :
    mosquittopp((const char *)id.c_str()){

    int keepalive = 60;
    connect(host.c_str(), port, keepalive);

    time_at = publish_every_ms;
    first_cycle = 0;
    mqtt_name = id;
    topic = id + "/send/temp";
    location = where;
  }


  void TEMPERATURE::tooop(unsigned int last_loop_ms){
    if (!first_cycle){
      time_ac = rand()%time_at;
      read();
      publish_now();
      first_cycle = 1;
    }
    time_ac += last_loop_ms;
    if (time_ac >= time_at){
      read();
      publish_now();
      time_ac = 0;
    }
    if(loop(15,1)){
      reconnect();
    }
  }


  void TEMPERATURE::read(){
    std::ifstream inFile(location.c_str(), std::ifstream::in);
    if (inFile.is_open()){
      getline(inFile, temp);
      inFile.close();
      temp = std::to_string(stof(temp)/1000.0f);

      D( topic << " Said temperature is: " << temp << std::endl);
    }
    else {
      std::cerr << "Error on TEMPERATURE::read(): " << strerror(errno) << std::endl;
    }
  }


  void TEMPERATURE::publish_now(){
    publish(NULL, topic.c_str(), temp.length() , temp.c_str());
  }


  void TEMPERATURE::on_connect(int rc){
    std::cout << "Connected with code "  <<  rc << " from: TEMPERATURE::" << mqtt_name << std::endl;
  }


  void TEMPERATURE::on_message(const struct mosquitto_message *message){
    std::cout << "no messages allowed" << std::endl;
  }


  void TEMPERATURE::on_subscribe(int mid, int qos_count, const int *granted_qos){
    std::cout << topic << " Subscription succeeded" << std::endl;
  }

  TEMPERATURE::~TEMPERATURE(){
    std::cout << "destroying TEMPERATURE::" << mqtt_name << std::endl;
  }
} /* namespace lighting */

//
// temperature.cpp ends here

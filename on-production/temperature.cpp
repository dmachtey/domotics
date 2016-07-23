// Filename: temperature.cpp
// 
// Description: 
// Author: Damian Machtey
// Maintainer: 
// 
// Created: 2015-06-22 Mon
// 
// Last-Updated: Sat Jul 23 12:44:35 2016 (-0500)
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


#include "temperature.h"
#include <iostream>
#include <fstream>
//#include <string.h>
//#include <math.h> // for log
#include <cstring>
#include <errno.h> // for errno


using namespace std;


namespace lighting{

  TEMPERATURE::TEMPERATURE(const char *id, const char *host, int port, char *name, char *where,
                           unsigned int publish_every_ms) : mosquittopp(id){

    //    mosqpp::lib_init(); // Initialize libmosquitto
    int keepalive = 60;
    connect(host, port, keepalive);

    time_at = publish_every_ms;
    first_cycle = 0;
    mqtt_name = name;
    topic = string(name) + "/send/temp";
    location = where;
  }


  void TEMPERATURE::tloop(unsigned int last_loop_ms){
    if (!first_cycle){
      time_ac = rand()%time_at;
      read();
      publishnow();
      first_cycle = 1;
    }
    time_ac += last_loop_ms;
    if (time_ac >= time_at){
      read();
      publishnow();
      time_ac = 0;
    }
    if(loop(15,1)){
      reconnect();
    }
  }

  void TEMPERATURE::read(){
    ifstream inFile(location.c_str(), ifstream::in);
    if (inFile.is_open()){
      getline(inFile, temp);
      inFile.close();
      temp = to_string(stof(temp)/1000.0f);
      //   cout << topic << " Said temperature is: " << temp << endl;
    }
    else {
      cerr << "Error on TEMPERATURE::read(): " << strerror(errno) << endl;
    }
  }

  void TEMPERATURE::publishnow(){
    publish(NULL, topic.c_str(), temp.length() , temp.c_str());
  }

  void TEMPERATURE::on_connect(int rc)
  {
    cout << "Connected with code "  <<  rc << " from: TEMPERATURE::" << mqtt_name << endl;
  }

  void TEMPERATURE::on_message(const struct mosquitto_message *message)
  {
    cout << "no messages allowed" << endl;
  }

  void TEMPERATURE::on_subscribe(int mid, int qos_count, const int *granted_qos)
  {
    cout << topic << " Subscription succeeded" << endl;
  }

  TEMPERATURE::~TEMPERATURE(){
    cout << "destroying TEMPERATURE::" << mqtt_name << endl;
  }
} /* namespace lighting */

// 
// temperature.cpp ends here

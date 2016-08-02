// Filename: temperature.h
// 
// Description: 
// Author: Damian Machtey
// Maintainer: 
// 
// Created: 2015-06-22 Mon
// 
// Last-Updated: Sat Jul 23 12:44:47 2016 (-0500)
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


#ifndef temperature_H_
#define temperature_H_
#include <string>

#include <mosquittopp.h>
#include <stdlib.h>

using std::string;

namespace lighting{

  class TEMPERATURE : public mosqpp::mosquittopp{
   private:
    string temp;
    unsigned int time_ac; // time accumulator
    unsigned int time_at; // publish
    unsigned int first_cycle;
    string topic;
    string mqtt_name;
    string location; // file location
  public:
    TEMPERATURE(const char *id, const char *host, int port, char *name, char *where,
                unsigned int publish_every_ms);
    void tloop(unsigned int last_loop_ms);
    string getTemp() {return temp;}
    double getTempd() {return atof(temp.c_str());}
    ~TEMPERATURE();
  private:
    void read();
    void publishnow();
    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
  };
} /* namespace lighting */

#endif /* temperature_H_ */

// 
// temperature.h ends here

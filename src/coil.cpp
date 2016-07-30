// Filename: coil.cpp
//
// Description: Implement the COIL class
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 12:04:29 2016(-0500)
//
// Last-Updated: Sat Jul 30 16:19:44 2016 (-0500)
//           By: Damian Machtey
//     Update #: 131

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


//#include <cstring> // memcpy, strcmp, strlen...
#include "coil.h"


namespace lighting{
  COIL::COIL(const char* id, const char* host, int port, const char* name,
             uint auto_off_time, double power) : mosquittopp(id)
  {
    time_off_sp = auto_off_time;
    mqtt_name = name;
    int keepalive = 60;
    connect(host, port, keepalive);
    republish_acc = rand()%(REPUBLISH_TIME);
    this->power = power;
    read_conf();
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
    if(republish_acc > REPUBLISH_TIME){
      publish_now();
      republish_acc = 0;
    }

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

  void COIL::on_connect(int rc){
    std::cout << "Connected with code: " << rc << " from: " << mqtt_name << std::endl;
    if(rc == 0){
      /* Only attempt to subscribe on a successful connect. */
      std::string sub = mqtt_name + "/#";
      subscribe(NULL, sub.c_str());
    }
  }

  void COIL::on_message(const struct  mosquitto_message *message){
    std::string search_msg = mqtt_name + "/get/status";
    if(!search_msg.compare(message->topic)){
      if(!(std::string("on")).compare((char *)message->payload)) goOn();
      else goOff();

      D("COIL is " << on << std::endl);
      return;
    }

    search_msg = mqtt_name + "/set/time_off_sp";
    if(!search_msg.compare(message->topic)){
      std::stringstream((char *)message->payload) >> time_off_sp;
      write_conf();
      return;
    }
  }

  void COIL::on_subscribe(int mid, int qos_count, const int* granted_qos)
  {
    std::cout << "Subscription succeeded from: " << mqtt_name << std::endl;
  }

  void COIL::publish_now()
  {
    std::string topic = mqtt_name + "/send/status" ;
    std::string payload = on ? "on" : "off";  // CFLAGS=-std=c++11 on Makefile
    publish(NULL, topic.c_str(), payload.length() , payload.c_str());

    D(mqtt_name << " is now " << std::boolalpha << on << std::endl);
    D("payload: "  << payload << std::endl);
    D("topic: " << topic << std::endl);
  }

  void COIL::compute_power(time_t scan_time)
  {
    D("to be implemented" << std::endl);
    // total_power
    // total_power_all_clases
  }


  void COIL::write_conf(){
    nlohmann::json config;
    config["time_off_sp"] = time_off_sp;
    std::ofstream configfile;
    std::string configfile_name = CONF_LOCATION + mqtt_name + ".json";

    configfile.open(configfile_name);
    if (configfile.is_open()){
      configfile << config.dump(4);
      configfile.close();
    }else{
      std::cerr << "file opening error on write at: " << configfile_name << std::endl;
    }
  }


  void COIL::read_conf(){
    nlohmann::json config;
    std::ifstream configfile;
    std::string configfile_name = CONF_LOCATION + mqtt_name + ".json";
    configfile.open(configfile_name, std::ofstream::in);
    if (configfile.is_open()){
      config << configfile;
      configfile.close();
      time_off_sp = config["time_off_sp"];
    }else{
      std::cerr << "file opening error on read from: " << configfile_name << std::endl;
    }
  }

} //namespace lighting
//
// COIL.cpp ends here

// Filename: dimmer.cpp
//
// Description: implement the DIMMER class. aims to control a light intensity
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 15:11:07 2016 (-0500)
//
// Last-Updated: Mon Aug 15 15:24:47 2016 (-0300)
//           By: Damian Machtey
//     Update #: 326

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

  DIMMER::DIMMER(std::string id, std::string host, int port,
                 double power, uint max_level, uint gpio,
                 PRULOADER *pru) : COIL (id, host, port, power){
    this->max_level = max_level; //LOOPCOUNTER
    this->pru = pru;
    this->gpio = gpio;
  }


  uint DIMMER::looop(unsigned int scan_time, bool sw, bool door_bell_sw){
    bool longpress = false;
    bool shortpress = false;
    uint next_slot = 0;
    if (!first_scan){
      read_conf();
      first_scan = true;
    }

    longpress = (sw_press_acc > MIN_SCALING_TIME);
    shortpress = ((sw_press_acc > 0) && !sw && !longpress);

    sw_press_acc += scan_time; //how long is the sw pressed.
    sw_press_acc *= (sw==true);

    ringing_acc += scan_time;
    ringing_acc *= (ringing_latch==true);

    // timeout, turn it off
    time_off_acc += scan_time * (time_off_sp>0);
    time_off_acc *=(on==true);
    if(time_off_acc > time_off_sp) going_off = true;

    // auto publish
    republish_acc += scan_time;
    if(republish_acc > REPUBLISH_TIME){
      publish_now();

      D("Republish from: " << mqtt_name << " duty at: "
        << duty << " republish acc: " << republish_acc << std::endl);

      republish_acc = 0;
    }

    next_slot = (sw_press_acc - MIN_SCALING_TIME)/SCALING_TIME; //for scaling on long press

    if (shortpress  && going_latch){
      going_on = false; //reset
      going_off = false; //reset
      // TODO STOP The threads

      D("all off now, shortpress, going_on, going_off\n");
      shortpress = false;
    }

    if (shortpress && !on && !going_latch) going_on = true; //set
    if (shortpress &&  on && !going_latch) going_off = true; //set


    if (going_on && !going_latch){
      t_going_on = new std::thread(DIMMER::goingOn, &duty, gpio, &going_on, max_level, pru);
            //  if(t_going_on.joinable()) t_going_on.join();
      going_latch = true;
    }
    if (going_off && !going_latch){
      t_going_off = new std::thread(goingOff, &duty, gpio, &going_off, pru);
      //      if(t_going_off.joinable()) t_going_off.join();
      going_latch = true;
    }

    if(!going_on && !going_off)
      going_latch = false;

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

    bool ringing = false;
    // Ring the light
    if (door_bell_sw || ringing_latch){
      duty = (((ringing_acc / 350) % 2) == 0)*70+15;
      ringing = true;
      ringing_latch = true;
      if (ringing_acc > 3000) ringing_latch = false;
      D("ringing at: " << duty << std::endl);
    }
    if (ringing && !ringing_latch)  // go back to the start value
      duty = old_duty;

    // limits control
    if (duty >= max_level) duty = max_level;
    if (duty <= 0) duty = 0;

    on = (duty > 0);

    // publish
    if (((duty != old_duty) || (republish_acc > REPUBLISH_TIME)) && !going_on && !going_off && !ringing_latch){
      old_duty = duty;
      republish_acc *= (republish_acc < REPUBLISH_TIME); //Reset the counter when published was forced
      publish_now();
    }


    if (!going_on && !going_off)
      pru->set_pwm(gpio, duty);
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


  void DIMMER::goingOff(uint *duty, uint gpio, bool *running, PRULOADER *pru){
    int pwm = *duty;
    do{
      pwm = *duty-1;
      *duty = pwm;
      pru->set_pwm(gpio, pwm);
      std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }while((pwm > 0) && *running);
    *running = false;
    D("goingOff finish\n";)
  }


  void DIMMER::goingOn(uint *duty, uint gpio, bool *running, uint max_level, PRULOADER *pru){
    uint pwm;
    do{
      *duty += 1;
      pwm = *duty;
      pru->set_pwm(gpio, pwm);
      std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }while((*duty < max_level) && *running);
    *running = false;
    D("goingOn finish\n";)
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
      int d = duty;
      d -= max_level/DIMMING_STEPS;
      if (d <= 0){
        d = max_level/DIMMING_STEPS;
        up_down = 1;
      }
      duty = d;
    } // down
  }


  void DIMMER::on_connect(int rc){
    std::cout << "Connected with code: " << rc << " from: " << mqtt_name << std::endl;
    if(rc == 0){
      /* Only attempt to subscribe on a successful connect. */
      std::string sub = mqtt_name + "/#";
      subscribe(NULL, sub.c_str());
    }
  }


  void DIMMER::publish_now(){
    std::string topic = mqtt_name + "/send/duty";
    std::string payload = std::to_string(duty);
    publish(NULL, topic.c_str(), payload.length() , payload.c_str());

    D(mqtt_name << " is now at: " << duty << std::endl);
    D("payload: "  << payload << std::endl);
    D("topic: " << topic << std::endl);

  }


  void DIMMER::on_message(const struct mosquitto_message *message){
    std::string search_msg = mqtt_name + "/get/duty";
    if(!search_msg.compare(message->topic)){
      std::stringstream((char *)message->payload) >> duty;
      time_off_acc = 0;
      return;
    }

    search_msg = mqtt_name + "/set/ringing";
    if(!search_msg.compare(message->topic)){
      ringing_latch = true;
      return;
    }

    search_msg = mqtt_name + "/set/time_off_sp";
    if(!search_msg.compare(message->topic)){
      std::stringstream((char *)message->payload) >> time_off_sp;
      write_conf();
      return;
    }

    search_msg = mqtt_name + "/set/max_level";
    if(!search_msg.compare(message->topic)){
      std::stringstream((char *)message->payload) >> max_level;
      if (max_level > LOOPCOUNTER) max_level = LOOPCOUNTER;
      write_conf();
      return;
    }

    search_msg = mqtt_name + "/set/going_on";
    if(!search_msg.compare(message->topic)){
      going_on = true;
      going_off = false;
      return;
    }

    search_msg = mqtt_name + "/set/going_off";
    if(!search_msg.compare(message->topic)){
      if(on) going_off = true;
      going_on = false;
      return;
    }
  }


  void DIMMER::write_conf(){
    nlohmann::json config;
    config["time_off_sp"] = time_off_sp;
    config["max_level"] = max_level;
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


  void DIMMER::read_conf(){
    nlohmann::json config;
    std::ifstream configfile;
    std::string configfile_name = CONF_LOCATION + mqtt_name + ".json";
    configfile.open(configfile_name, std::ofstream::in);
    if (configfile.is_open()){
      config << configfile;
      configfile.close();
      time_off_sp = config["time_off_sp"];
      max_level = config["max_level"];
      D(config.dump(4));
    }else{
      std::cerr << "file opening error on read from: " << configfile_name << std::endl;
      write_conf();
    }
  }
}//namespace
//
// DIMMER.cpp ends here

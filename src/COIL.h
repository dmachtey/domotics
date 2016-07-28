// Filename: COIL.h
//
// Description: Implement a COIL control class
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 11:58:39 2016(-0500)
//
// Last-Updated: Thu Jul 28 11:35:45 2016 (-0500)
//           By: Damian Machtey
//     Update #: 31

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

#ifndef COIL_H
#define COIL_H
#include "mosquittopp.h"
#include "DOMTYPES.h"
#include <string>

namespace lighting {


  /**
   * class COIL
   * Implement a coil control class
   */
  class COIL : public mosqpp::mosquittopp{
  public:

    /**
     * COIL Constructor
     * @param  id
     * @param  host
     * @param  port
     * @param  name
     * @param  auto_off_time
     * @param  power
     */
    COIL(const char* id, const char* host, int port, const char* name, uint auto_off_time, double power);


    /**
     * This function must be called in an infinite loop
     * It take care of all the functionality of the class
     *
     * @return bool the status of the coil
     * @param  scan_time is the time from last call
     * @param  sw is the physical switch
     */
    bool looop(uint scan_time, bool sw);


    bool get_on() const{
      return on;
    }

    void goOn();

    void goOff();

    void set_time_off_sp(const time_t time_off_sp){
      this->time_off_sp = time_off_sp;
    }

  protected:
    // Static Protected attributes
    //

    // Aggregated consumption of all objects [W/h]
    // need to be initialized once,
    // need also to decide what to do with the collected info
    static double total_power_all_clases;

    // Protected attributes
    //

    // State of the coil
    bool on = false;
    // Track of how long has pass since the last command received [ms]
    // to switch it off automatically
    time_t time_off_acc = 0;
    // Set point to switch off automatically
    time_t time_off_sp = 0;
    // track how long the sw has been pressed [ms]
    time_t sw_press_acc = 0;
    // MQTT connection name
    std::string mqtt_name = 0;
    // Re-publish accumulator [ms]
    time_t republish_acc = 0;
    // Power controlled by this coil [W]
    double power = 0.0;
    // Actual consumption [W/h]
    double actual_power = 0.0;
    // Acumulative power consumed by this coil [W]
    double total_power = 0.0;

    /**
     * This function is the implementation of mosquittopp
     * Its call when connections is stablished
     *
     * @param  rc
     */
    void on_connect(int rc);


    /**
     * This function is the implementation of mosquittopp
     * is called every time a message is received
     *
     * @param  message
     */
    void on_message(const struct mosquitto_message *message);


    /**
     * This function is the implementation of mosquittopp
     * is called after subscription
     *
     * @param  mid
     * @param  qos_count
     * @param  granted_qos
     */
    void on_subscribe(int mid, int qos_count, const int* granted_qos);


    /**
     * Publish object status to moquitto server
     */
    void publish_now();


    /**
     * Compute the consumed power
     *
     * @param  scan_time
     */
    void compute_power(time_t scan_time);
  };


}; // end of package namespace

#endif // COIL_H

//
// COIL.h ends here

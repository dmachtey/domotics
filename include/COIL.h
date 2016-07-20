// Filename: COIL.h
// 
// Description: 
// Author: Damian Machtey
// Maintainer: 
// 
// Created: Wed Jul 20 16:35:29 2016 (-0500)
// 
// Last-Updated: Wed Jul 20 16:35:37 2016 (-0500)
//           By: Damian Machtey
//     Update #: 1

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



#ifndef COIL_H
#define COIL_H
#include "undef.h"
#include "undef.h"
#include "mosqpp/mosquittopp.h"

#include <string>
using mosqpp::mosquittopp;

namespace lighting {


/**
  * class COIL
  * Implement a coil class
  */

class COIL : public undef, public undef, public mosquittopp
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  COIL ();

  /**
   * Empty Destructor
   */
  virtual ~COIL ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  


protected:

  // Static Protected attributes
  //  

  // Protected attributes
  //  

  // State of the coil
  bool on;
  // Track of how long has pass since the last command received [ms]
  unsigned int time_off_acc;
  // track how long the sw has been pressed [ms]
  unsigned int sw_press_acc;
  // MQTT connection name
  string mqtt_name;
  // Re-publish accumulator [ms]
  unsigned int publish_acc;
  // Re-publish accumulator set point [ms]
  unsigned int publish_acc_sp;
  // Power controlled by this coil [W]
  unsigned int power;
  // Actual consumption [W/h]
  double actual_power;
  // Acumulative power consumed by this coil [W]
  double total_power;
public:


  // Protected attribute accessor methods
  //  

protected:

public:


  // Protected attribute accessor methods
  //  


  /**
   * Set the value of on
   * State of the coil
   * @param new_var the new value of on
   */
  void setOn (bool new_var)   {
      on = new_var;
  }

  /**
   * Get the value of on
   * State of the coil
   * @return the value of on
   */
  bool getOn ()   {
    return on;
  }

  /**
   * Set the value of time_off_acc
   * Track of how long has pass since the last command received [ms]
   * @param new_var the new value of time_off_acc
   */
  void setTime_off_acc (unsigned int new_var)   {
      time_off_acc = new_var;
  }

  /**
   * Get the value of time_off_acc
   * Track of how long has pass since the last command received [ms]
   * @return the value of time_off_acc
   */
  unsigned int getTime_off_acc ()   {
    return time_off_acc;
  }

  /**
   * Set the value of sw_press_acc
   * track how long the sw has been pressed [ms]
   * @param new_var the new value of sw_press_acc
   */
  void setSw_press_acc (unsigned int new_var)   {
      sw_press_acc = new_var;
  }

  /**
   * Get the value of sw_press_acc
   * track how long the sw has been pressed [ms]
   * @return the value of sw_press_acc
   */
  unsigned int getSw_press_acc ()   {
    return sw_press_acc;
  }

  /**
   * Set the value of mqtt_name
   * MQTT connection name
   * @param new_var the new value of mqtt_name
   */
  void setMqtt_name (string new_var)   {
      mqtt_name = new_var;
  }

  /**
   * Get the value of mqtt_name
   * MQTT connection name
   * @return the value of mqtt_name
   */
  string getMqtt_name ()   {
    return mqtt_name;
  }

  /**
   * Set the value of publish_acc
   * Re-publish accumulator [ms]
   * @param new_var the new value of publish_acc
   */
  void setPublish_acc (unsigned int new_var)   {
      publish_acc = new_var;
  }

  /**
   * Get the value of publish_acc
   * Re-publish accumulator [ms]
   * @return the value of publish_acc
   */
  unsigned int getPublish_acc ()   {
    return publish_acc;
  }

  /**
   * Set the value of publish_acc_sp
   * Re-publish accumulator set point [ms]
   * @param new_var the new value of publish_acc_sp
   */
  void setPublish_acc_sp (unsigned int new_var)   {
      publish_acc_sp = new_var;
  }

  /**
   * Get the value of publish_acc_sp
   * Re-publish accumulator set point [ms]
   * @return the value of publish_acc_sp
   */
  unsigned int getPublish_acc_sp ()   {
    return publish_acc_sp;
  }

  /**
   * Set the value of power
   * Power controlled by this coil [W]
   * @param new_var the new value of power
   */
  void setPower (unsigned int new_var)   {
      power = new_var;
  }

  /**
   * Get the value of power
   * Power controlled by this coil [W]
   * @return the value of power
   */
  unsigned int getPower ()   {
    return power;
  }

  /**
   * Set the value of actual_power
   * Actual consumption [W/h]
   * @param new_var the new value of actual_power
   */
  void setActual_power (double new_var)   {
      actual_power = new_var;
  }

  /**
   * Get the value of actual_power
   * Actual consumption [W/h]
   * @return the value of actual_power
   */
  double getActual_power ()   {
    return actual_power;
  }

  /**
   * Set the value of total_power
   * Acumulative power consumed by this coil [W]
   * @param new_var the new value of total_power
   */
  void setTotal_power (double new_var)   {
      total_power = new_var;
  }

  /**
   * Get the value of total_power
   * Acumulative power consumed by this coil [W]
   * @return the value of total_power
   */
  double getTotal_power ()   {
    return total_power;
  }
protected:



  /**
   * @param  rc
   */
  void on_connect (int rc)
  {
  }


  /**
   * @param  message
   */
  void on_message (undef message)
  {
  }


  /**
   * @param  mid mosquittopp
   * @param  qos_count mosquittopp
   * @param  granted_qos mosquittopp
   */
  void on_subscribe (int mid, int qos_count, const int* granted_qos)
  {
  }


  /**
   */
  void publish_now ()
  {
  }


  /**
   * @param  scan_time Last scan time [ms]
   */
  void power (unsigned int scan_time)
  {
  }

private:

  // Static Private attributes
  //  

  // Agregated consumption of all clases [W/h], need to be initialized once, need also to decide what to do with the collected info
  static doble total_power_all_clases;
  // Private attributes
  //  

public:


  // Private attribute accessor methods
  //  


  /**
   * Get the value of total_power_all_clases
   * Agregated consumption of all clases [W/h], need to be initialized once, need
   * also to decide what to do with the collected info
   * @return the value of total_power_all_clases
   */
  doble getTotal_power_all_clases ()   {
    return COIL::total_power_all_clases;
  }
private:

public:


  // Private attribute accessor methods
  //  

private:



  /**
   * @param  id mosquittopp
   * @param  host mosquittopp
   * @param  port mosquittopp
   * @param  name mosquittopp
   * @param  auto_off_time if 0 no auto_off [ms]
   * @param  power Controlled power [W]
   */
   COIL (const char* id, const char* host, int port, const char* name, unsigned int auto_off_time, unsigned int power)
  {
  }

  void initAttributes () ;

};
}; // end of package namespace

#endif // COIL_H

// 
// COIL.h ends here

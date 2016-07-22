
#ifndef COIL_H
#define COIL_H
#include "mosquittopp.h"

#include <string>

namespace lighting {


/**
  * class COIL
  * class COIL
  * class COIL
  * Implement a coil class
  */

class COIL : public mosquittopp
{
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
   COIL (const char* id,
         const char* host,
         int port, const char* name,
         unsigned int auto_off_time,
         unsigned int power);


  /**
   * This function must be called in an infinite loop
   * It take care of all the functionality of the class
   *
   * @return bool
   * @param  scan_time
   * @param  sw
   */
  bool looop (unsigned int scan_time,
              bool sw);

protected:

  // Static Protected attributes
  //

  // Aggregated consumption of all clases [W/h], need to be initialized once, need also to decide what to do with the collected info
  static doble total_power_all_clases;
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

protected:

public:

protected:



  /**
   * This function is the implementation of mosquittopp
   * Its call when connections is stablished
   *
   * @param  rc
   */
  void on_connect (int rc);


  /**
   * This function is the implementation of mosquittopp
   * is called every time a message is received
   *
   * @param  message
   */
  void on_message (struct const mosquitto_message* message);


  /**
   * This function is the implementation of mosquittopp
   * is called after subscription
   *
   * @param  mid
   * @param  qos_count
   * @param  granted_qos
   */
  void on_subscribe (int mid,
                     int qos_count,
                     const int* granted_qos);


  /**
   * Publish object status to moquitto server
   */
  void publish_now ();


  /**
   * Compute the consumed power
   *
   * @param  scan_time
   */
  void power (unsigned int scan_time);

private:

public:

private:

public:

private:



};
}; // end of package namespace

#endif // COIL_H

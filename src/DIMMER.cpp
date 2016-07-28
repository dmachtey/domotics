// Filename: DIMMER.cpp
//
// Description: implement the DIMMER class. aims to control a light intensity
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 15:11:07 2016 (-0500)
//
// Last-Updated: Mon Jul 25 23:55:44 2016 (-0500)
//           By: Damian Machtey
//     Update #: 14

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

#include "DIMMER.h"

namespace lighting
{

  /**
   * DIMMER Constructor
   * @param  id
   * @param  host
   * @param  port
   * @param  name
   * @param  auto_off_time
   * @param  power
   */
  DIMMER::DIMMER (const char* id, const char* host, int port, const char* name,
              uint auto_off_time, double power) : COIL (id, host, port, name,
                                                        auto_off_time, power)
  {
    // on = false;
    // time_off_acc = 0;
    // time_off_sp = auto_off_time;
    // sw_press_acc = 0;
    // mqtt_name = name;
    // int keepalive = 60;
    // connect(host, port, keepalive);
    // publish_now();
    // republish_acc = rand()%(REPUBLISH_TIME);
    // this->power = power;
  }

  /**
   * @name loop -
   * @param scan_time - Number of scan time
   * @param sw -  sw
   * @param door_bell_sw -  door bell sw
   * @return uint
   */
  uint DIMMER::loop (unsigned int scan_time, bool sw, bool door_bell_sw)
  {

  }


  /**
   * @name goOffNow -
   * @return void
   */
  void DIMMER::goOffNow ()
  {
    duty = 0;
    on = false;
  }


  /**
   * @name goOnNow -
   * @return void
   */
  void DIMMER::goOnNow ()
  {
    duty = max_level;
    on = true;
  }


  /**
   * @name goOn -
   * @return void
   */
  void DIMMER::goOn ()
  {
    going_on = true;
    going_off = false;
  }


  /**
   * @name goOff -
   * @return void
   */
  void DIMMER::goOff ()
  {
    going_off = true;
    going_on = false;
  }


  /**
   * @name goingOff -
   * @return void
   */
  void DIMMER::goingOff ()
  {
  }


  /**
   * @name GoingON -
   * @return void
   */
  void DIMMER::GoingON ()
  {
  }


  /**
   * @name fading -
   * @return void
   */
  void DIMMER::fading ()
  {
  }


}//namespace
//
// DIMMER.cpp ends here

// Filename: dimmer.h
//
// Description: DIMMER class header. aims to control a light intensity
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 15:12:52 2016 (-0500)
//
// Last-Updated: Thu Aug 11 15:12:49 2016 (-0500)
//           By: Damian Machtey
//     Update #: 49

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

#ifndef DIMMER_H
#define DIMMER_H

#include <thread>
#include <cstring>
#include <string>
#include <iostream>

#include "coil.h"
#include "domtypes.h"
#include "pru_data.hp"

namespace lighting {

  class DIMMER : public COIL{
  public:

    /**
     * DIMMER Constructor
     * @param  id
     * @param  host
     * @param  port
     * @param  name
     * @param  auto_off_time
     * @param  power
     */
    DIMMER(std::string id, std::string host, int port,
           double power, uint max_level, uint gpio, void (*set_pww)(uint, uint));

    /**
     * @name loop - should be called in an infinite loop
     * It mean to control a light intensity
     * by mqtt messages
     * or by a physical switch following the next table:
     * | press | is        | start     | on            |
     * |-------+-----------+-----------+---------------|
     * | short | off       | going on  | release       |
     * | short | on        | going_off | release       |
     * | short | going_on  | stop      | release       |
     * | short | going_off | stop      | release       |
     * | long  | x         | fade      | while pressed |
     * @param scan_time -  scan time
     * @param sw -  sw
     * @param door_bell_sw -  door bell sw
     * @return uint
     */
    uint looop(time_t scan_time, bool sw, bool door_bell_sw);


    /**
     */
    void swOffNow();


    /**
     */
    void swOnNow();


    /**
     */
    void goOn();


    /**
     */
    void goOff();


  private:
    /**
     */
    void goingOff();


    /**
     */
    void goingOn();


    /**
     */
    void fading();


    void on_connect(int rc);


    void publish_now();


    void on_message(const struct mosquitto_message *message);


    /**
     * @name write_conf - Writes config file
     * @return void
     */
    void write_conf();


    /**
     * @name read_conf - Reads config file
     * @return void
     */
    void read_conf();

    // Private attributes
    //

    // duty of PWM
    lighting::time_t duty = 0;
    // Publish only on changes
    lighting::time_t old_duty = 0;
    // When you keep press the sw it start fadding
    // first it goes down until a minimum is eached
    // them it change up
    bool up_down = false;
    // PWM levels
    uint max_level = 100;
    // keep track of fadding steps
    uint sw_slots = 0;
    // when ring command is gotten
    // it control the lenght of ringging sequence
    lighting::time_t ringing_acc = 0;
    // lenght of fading steps
    lighting::time_t fading_acc = 0;
    // Active when ringing sequence is active
    // together with ringing_latch are used to
    // return to old_duty when ringing sequece is over
    // bool ringing = false;  //< could be local! TODO
    // Active when ringing sequence is active
    // together with ringing_latch are used to
    // return to old_duty when ringing sequece is over
    bool ringing_latch = false;
    // A short pressed was detected and we were "!on"
    // we are turning the lights on
    bool going_on = false;
    // A short pressed was detected and we were "on"
    // wee are turning the lights off
    bool going_off = false;
    // going_on and off steps accumulator
    lighting::time_t going_on_off_acc = 0;
    void (*Set_PWM)(uint, uint);
  };
}; // end of package namespace
#endif // DIMMER_H
//
// DIMMER.h ends here

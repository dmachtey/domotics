// Filename: DIMMER.h
// 
// Description: DIMMER class header. aims to control a light intensity
// Author: Damian Machtey
// Maintainer: 
// 
// Created: Mon Jul 25 15:12:52 2016 (-0500)
// 
// Last-Updated: Mon Jul 25 15:32:21 2016 (-0500)
//           By: Damian Machtey
//     Update #: 6

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
#include "COIL.h"

#include <string>
#include "DOMTYPES.h"

namespace lighting {

  class DIMMER : public COIL
  {
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
    DIMMER (const char* id, const char* host, int port, const char* name, uint auto_off_time, double power);
  
    /**
     * @name loop - 
     * @param scan_time -  scan time 
     * @param sw -  sw 
     * @param door_bell_sw -  door bell sw 
     * @return uint
     */
    uint loop (time_t scan_time, bool sw, bool door_bell_sw);


    /**
     */
    void goOffNow ();


    /**
     */
    void goOnNow ();


    /**
     */
    void goOn ();


    /**
     */
    void goOff ();


    /**
     */
    void goingOff ();


    /**
     */
    void GoingON ();


    /**
     */
    void fading ();

  private:

    // Private attributes
    //  

    // duty of PWM 
    unsigned int duty;
    // Publish only on changes
    unsigned int old_duty;
    // When you keep press the sw it start fadding
    // first it goes down until a minimum is eached
    // them it change up
    bool up_down;
    // PWM levels
    unsigned int max_level;
    // keep track of fadding steps
    unsigned int sw_slots;
    // when ring command is gotten
    // it control the lenght of ringging sequence 
    unsigned int ringing_acc;
    // lenght of fading steps
    unsigned int fading_acc;
    // Active when ringing sequence is active
    // together with ringing_latch are used to 
    // return to old_duty when ringing sequece is over
    bool ringing;
    // Active when ringing sequence is active
    // together with ringing_latch are used to 
    // return to old_duty when ringing sequece is over
    bool ringing_latch;
    // A short pressed was detected and we were "!on"
    // we are turning the lights on
    // 
    bool going_on;
    // A short pressed was detected and we were "on"
    // we are turning the lights off
    bool going_off;

  };
}; // end of package namespace
#endif // DIMMER_H

// 
// DIMMER.h ends here

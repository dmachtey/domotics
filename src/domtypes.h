// Filename: domtypes.h
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 11:51:24 2016 (-0500)
//
// Last-Updated: Wed Aug 10 07:04:56 2016 (-0500)
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
#ifndef _MODTYPES_H_
#define _MODTYPES_H_

#include <string>

#define DEBUG
#ifdef DEBUG
#define D(x) std::cout << x
#else
#define D(x)
#endif

#define REPUBLISH_TIME 2*60*1000 // 2 minutes
#define MIN_SCALING_TIME 1000 // ms to start dimming (less shortpress)
#define SCALING_TIME 500 // ms for each dimming step
#define DIMMING_STEPS 20 // number of dimming steps
// where the configurations files are stored
#define CONF_LOCATION "/home/dom/"


namespace lighting {

  typedef unsigned int time_t;
  typedef unsigned int uint;


  typedef enum {
    LED,
    LIGTH,
    OUTLET,
    FAN,
    MOTOR,
    OTHER
  } p_device;


  class powered {
  public:
    int id;
    std::string name;
    std::string placed;
    double power;
    double voltage;
    uint n_circuits;
    p_device type;
  };


  typedef enum {
    zero = 0,
    one = 1
  } pru_num;


}
#endif //DOMTYPES_H_
//
// DOMTYPES.h ends here

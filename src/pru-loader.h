// Filename: pru-loader.h
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Thu Aug  4 12:51:43 2016 (-0500)
//
// Last-Updated: Fri Aug  5 20:17:30 2016 (-0500)
//           By: Damian Machtey
//     Update #: 15

// Change Log:
//
//
// Copyright (C) 2016 Damian Machtey
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//
//

// Code:
#ifndef PRU-LOADER_H
#define PRU-LOADER_H

#include <iostream>
#include <string>
#include <unistd.h> //for usleep, getudi
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "domtypes.h"
#include "pru_data.hp"

namespace lighting{
  class PRULOADER{
  prublic:
    PRULOADER(std::string prubinary, pru_num pru);
    void set_pwm(uint gpio, uint pwm);

  private:
    static void *pruDataMemory;
    static unsigned int *pruDataMemory_int;
    int pru_num = 0;
  }
} //namespace
#endif // PRU-LOADER_H
//
// pru-loader.h ends here

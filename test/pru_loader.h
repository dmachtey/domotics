// Filename: pru_loader.h
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Thu Aug  4 12:51:43 2016 (-0500)
//
// Last-Updated: Fri Jun 30 17:03:08 2017 (-0300)
//           By: Damian Machtey
//     Update #: 29

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
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//

// Code:
#ifndef PRU_LOADER_H
#define PRU_LOADER_H

#include <iostream>

#include <string>
#include <unistd.h> //for usleep, getudi
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "domtypes.h"
#include "pru_data.hp"

namespace lighting{
  class PRULOADER{
  public:
    PRULOADER(std::string prubinary, pru_num pru);
    void set_pwm(uint gpio, uint pwm);
    void print();
    ~PRULOADER();
  private:
    void *pruDataMemory;
    unsigned int *pruDataMemory_int;
    pru_num prunum;
  };
} //namespace
#endif // PRU-LOADER_H
//
// pru_loader.h ends here

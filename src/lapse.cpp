// Filename: lapse.cpp
//
// Description: Compute the time lapse between calls in milliseconds
// Author: Damian Machtey
// Maintainer:
//
// Created: Thu Jul 28 11:03:14 2016 (-0500)
//
// Last-Updated: Fri Jun 30 17:14:02 2017 (-0300)
//           By: Damian Machtey
//     Update #: 19

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
#include "lapse.h"

namespace lighting{

  LAPSE::LAPSE(){
    previus_tick = std::chrono::steady_clock::now();
  }

  lighting::time_t LAPSE::get_lapse(){
      std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();
      uint rtn = std::chrono::duration_cast<std::chrono::nanoseconds>(now - previus_tick).count();
      previus_tick = now;
      return rtn/1000000;
    }

} //namespace
//
// lapse.cpp ends here

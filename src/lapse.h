// Filename: lapse.h
//
// Description: Compute the time lapse between calls in milliseconds
// Author: Damian Machtey
// Maintainer:
//
// Created: Thu Jul 28 10:48:16 2016 (-0500)
//
// Last-Updated: Fri Jun 30 17:03:27 2017 (-0300)
//           By: Damian Machtey
//     Update #: 21

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

#ifndef LAPSE_H
#define LAPSE_H

#include <chrono>
#include "domtypes.h"


namespace lighting{

    /**
   * class LAPSE
   * Implement a coil control class
   */

  class LAPSE{
  private:
    std::chrono::steady_clock::time_point previus_tick;

  public:

    LAPSE();
    lighting::time_t get_lapse();
  };




} // namespace lighting
//
#endif // LAPSE_H
// lapse.h ends here

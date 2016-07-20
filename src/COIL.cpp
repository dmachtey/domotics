// Filename: COIL.cpp
// 
// Description: 
// Author: Damian Machtey
// Maintainer: 
// 
// Created: Wed Jul 20 16:34:45 2016 (-0500)
// 
// Last-Updated: Wed Jul 20 16:35:00 2016 (-0500)
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

#include "COIL.h"

// Constructors/Destructors
//  

COIL::COIL () {
initAttributes();
}

COIL::~COIL () { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

void COIL::initAttributes () {
  on = false;
  time_off_acc = 0;
  sw_press_acc = 0;
  publish_acc = 0;
  publish_acc_sp = 0;
  power = 0;
  actual_power = 0.0;
  total_power = 0.0;
  total_power_all_clases = 0.0;
}


// 
// COIL.cpp ends here

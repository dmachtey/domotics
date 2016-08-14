// Filename: direct-pru.cpp
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Fri Aug 12 17:40:46 2016 (-0300)
//
// Last-Updated:
//           By:
//     Update #: 16

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
// Compile: g++ -std=c++11 pru_loader.cpp direct-pru.cpp -lprussdrv -pthread
#include <iostream>
#include "pru_loader.h"
#include "domtypes.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace lighting;

int main(int argc, char *argv[])
{
  // Configure and load pruss program
  PRULOADER pru_0("/usr/lib/domotics/pwm.bin", zero);

  bool toggle = true;
  unsigned int  p =0;
  do{
    if (toggle)
      p += 1;
    else
      p -= 1;

    if(p >= 100){
      p = 100;
      toggle = false;
    }
    if(p <= 0){
      p = 0;
      toggle = true;
    }

    pru_0.set_pwm(GPIO1_15_SP, p);
    std::this_thread::sleep_for(std::chrono::milliseconds(24));
  }while(1);


  return 0;
}


//
// direct-pru.cpp ends here

// Filename: prus_delay.cpp
// 
// Description: 
// Author: Damian Machtey
// Maintainer: 
// 
// Created: 2015-06-22 Mon 
// 
// Last-Updated: Sat Jul 23 12:43:51 2016 (-0500)
//           By: Damian Machtey
//     Update #: 2

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



#include <iostream>

#define LOOPCOUNTER  100
#define INS_PER_US   200         // 5ns per instruction
#define INS_PER_DELAY_LOOP 2     // two instructions per delay loop

#define INS_PER_GPIO 12
#define TARGET_HZ 1333
#define N_GPIOS 33
#define TARGET_LOOP_TIME (1000000 / TARGET_HZ)  // us that should last each main loop

#define INS_OVERHEAD 4


// DELAY_INS = (TargetTimePerLoop - Needed Time per Loop)/LoopCounter


#define DELAY_T_USED ((INS_PER_GPIO * N_GPIOS + INS_OVERHEAD) * LOOPCOUNTER)/INS_PER_US // time us consumed by program

#define DELAY_T  (TARGET_LOOP_TIME - DELAY_T_USED)

#define DELAY_INS ((DELAY_T * INS_PER_US) / (LOOPCOUNTER * INS_PER_DELAY_LOOP))


using namespace std;

int main(int argc, char *argv[])
{
  cout << endl;
  cout << "Loopcounter/duty points: " << LOOPCOUNTER << endl;
  cout << "Target frequency: " << TARGET_HZ << " Hz" <<  endl << endl;


  cout << "Target loop time: " << TARGET_LOOP_TIME << " us" << endl;
  cout << "Delay time: " << DELAY_T << " us" << endl;
  cout << "Delay inst: " <<  DELAY_INS << endl;
  cout << "Max frequency: " << 1e6*INS_PER_US/((INS_PER_GPIO * N_GPIOS + INS_OVERHEAD) * LOOPCOUNTER) << " Hz" << endl;

  return 0;
}

// 
// prus_delay.cpp ends here

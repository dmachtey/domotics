// Filename: main.cpp
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 11:44:00 2016 (-0500)
//
// Last-Updated: Thu Jul 28 11:44:05 2016 (-0500)
//           By: Damian Machtey
//     Update #: 59

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
#include <time.h>
#include <unistd.h> //for usleep

//kbhit()
#include <stdlib.h>
#include <ncurses.h>

#include "COIL.h"
#include "DIMMER.h"
#include "DOMTYPES.h"
#include "lapse.h"

using namespace lighting;

unsigned int diff(timespec start, timespec end)
{ // returns milliseconds
  unsigned int temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp = end.tv_sec-start.tv_sec-1;
    temp = (1000000000+end.tv_nsec-start.tv_nsec)/1000000 + temp*1000;
  } else {
    temp = end.tv_sec-start.tv_sec;
    temp = (end.tv_nsec-start.tv_nsec)/1000000 + temp*1000;
  }
  return temp;
}


int main(int argc, char *argv[])
{

  LAPSE lapse;
  usleep(2000000);

  std::cout << "the delay was: " << lapse.get_lapse() << std::endl;
  return 0;



  std::cout.setf(std::ios::boolalpha);

  /* initialize random seed: */
  srand (time(NULL));

  mosqpp::lib_init();


   COIL C1((char *)"Coil1", (char *)"localhost", 1883, (char *)"Coil1", 10*1000, 18);
  DIMMER D1((char *)"DIM1", (char *)"localhost", 1883, (char *)"Coil1", 10*1000, 18);

  lighting::time_t scan_time;
  timespec prevt, actualt;
  clock_gettime(CLOCK_MONOTONIC, &actualt);
  prevt = actualt;


  // get keyboard value
  WINDOW *win;
  win = initscr(); // new screen will be created
  if (nodelay(win, TRUE)){
    std::cout << "error\n";
    return 0;
  }
  noecho();
  bool key;
  // end keyboard value

  powered LL1;
  LL1.name = "techo del quincho";
  LL1.power = 18*4;
  LL1.n_circuits = 4;
  LL1.type = LED;
  LL1.voltage = 60;

  do
    {
      clock_gettime(CLOCK_MONOTONIC, &actualt);
      scan_time = diff(prevt, actualt);
      prevt = actualt;


      unsigned int ch = getch();
      if (ch == 'a') //a
        key = true;
      if (ch == 's')
        key = false;
      if (ch == 'q')
        break;

      D1.looop(scan_time, key);
      //   key = false;

      usleep(10000);
    }while(true);

  endwin();

  std::cout << "finish" << std::endl;
  std::cout <<  D1.get_on() << std::endl;


  return 0;
}


//
// main.cpp ends here

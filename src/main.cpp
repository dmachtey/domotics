// Filename: main.cpp
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 11:44:00 2016 (-0500)
//
// Last-Updated: Sat Jul 30 14:57:22 2016 (-0500)
//           By: Damian Machtey
//     Update #: 95

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
//#include <time.h>
#include <unistd.h> //for usleep

//kbhit()
#include <stdlib.h>
#include <ncurses.h>

#include "coil.h"
#include "dimmer.h"
#include "domtypes.h"
#include "lapse.h"

using namespace lighting;


int main(int argc, char *argv[])
{

  /* initialize random seed: */
  srand (time(NULL));

  mosqpp::lib_init();


   COIL C1((char *)"Coil1", (char *)"localhost", 1883, (char *)"Coil1", 10*1000, 18);
   //DIMMER D1((char *)"DIM1", (char *)"localhost", 1883, (char *)"Dim1", 10*1000, 18, 100);

  lighting::time_t scan_time;

  // get keyboard value
  // WINDOW *win;
  // win = initscr(); // new screen will be created
  // if (nodelay(win, TRUE)){
  //   std::cout << "error\n";
  //   return 0;
  // }
  // noecho();
   bool key;
  // end keyboard value


  LAPSE lapse;
  do
    {
      scan_time = lapse.get_lapse();
      //   key = false;
      unsigned int ch = getch();
      if (ch == 'a') //a
        key = true;
      if (ch == 's')
        key = false;
      if (ch == 'q')
        break;

      //D1.looop(scan_time, key, false);
      C1.looop(scan_time, key);
      usleep(10000);

    }while(true);

  endwin();

  std::cout << "finish" << std::endl;
  std::cout.setf(std::ios::boolalpha);
  std::cout <<  C1.get_on() << std::endl;


  return 0;
}


//
// main.cpp ends here

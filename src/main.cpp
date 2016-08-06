// Filename: main.cpp
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 11:44:00 2016 (-0500)
//
// Last-Updated: Thu Aug  4 22:55:24 2016 (-0500)
//           By: Damian Machtey
//     Update #: 134

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
#include "pru_data.hp"
#include "lapse.h"
#include "temperature.h"

using namespace lighting;

bool COIL::master_set = false;

int main(int argc, char *argv[])
{

  // check pru configuration
  D("DELAY_INS = " << DELAY_INS << std::endl;)
  if(DELAY_INS < 0){
    std::cerr << "Error: there is a miss configuration on PRUS DELAY_INS, its < 0\n";
    exit(EXIT_FAILURE);
  }

  /* initialize random seed: */
  srand (time(NULL));
  mosqpp::lib_init();

  DIMMER D1("Dim1", "localhost", 1883, 18, 100);
  COIL C1("Coil1", "localhost", 1883, 18);
  TEMPERATURE Tmicro("micro", "localhost", 1883, "/some/where/", 2*60*1000);

  lighting::time_t scan_time;

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

      D1.looop(scan_time, key, false);
      C1.looop(scan_time, key);
      usleep(1000);

    }while(true);

  endwin();

  std::cout << "finish" << std::endl;
  std::cout.setf(std::ios::boolalpha);
  std::cout <<  C1.get_on() << std::endl;


  return 0;
}
//
// main.cpp ends here

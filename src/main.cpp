// Filename: main.cpp
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 11:44:00 2016 (-0500)
//
// Last-Updated: Wed Aug 10 08:33:33 2016 (-0500)
//           By: Damian Machtey
//     Update #: 176

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
//#include <unistd.h> //for usleep
#include <chrono>
#include <thread>
#include <csignal>

#include "mosquittopp.h"

#include "coil.h"
#include "dimmer.h"
#include "domtypes.h"
#include "pru_data.hp"
#include "lapse.h"
#include "temperature.h"
#include "pru_loader.h"
#include "gpio.h"

int finish_signal = 0;
void signal_handler(int signal){
  finish_signal = 1;
}

using namespace lighting;
bool COIL::master_set = false;

int main(int argc, char *argv[])
{

  D("DELAY_INS = " << DELAY_INS << std::endl;)
    if(DELAY_INS < 0){
      std::cerr << "Error: there is a miss configuration on PRUS DELAY_INS, its < 0\n";
      exit(EXIT_FAILURE);
    }

  // initialize random seed:
  srand (time(NULL));
  // initialize mosquittopp
  mosqpp::lib_init();


  // Configure and load pruss program
  PRULOADER pru_0("/usr/lib/domotics/pwm.bin", zero);


  // Inputs declaration
  exploringBB::GPIO SW1(4);   // patio
  exploringBB::GPIO SW2(51);  // salon
  exploringBB::GPIO SW3(50);  // ingreso
  exploringBB::GPIO SW4(60);  // baño
  exploringBB::GPIO SW5(115); // Cocina
  exploringBB::GPIO BELL(14); // Timbre

  SW1.setDirection(exploringBB::INPUT);
  SW2.setDirection(exploringBB::INPUT);
  SW3.setDirection(exploringBB::INPUT);
  SW4.setDirection(exploringBB::INPUT);
  SW5.setDirection(exploringBB::INPUT);
  BELL.setDirection(exploringBB::INPUT);

  // Outputs declaration
  DIMMER patio  ("LT1", "localhost", 1883, 18*4, 100);  // Patio
  DIMMER salon  ("LT2", "localhost", 1883, 18*4, 100);  // Salon quincho
  DIMMER ingreso("LT3", "localhost", 1883, 18,   100);  // Ingreso quincho
  DIMMER bano   ("LT4", "localhost", 1883, 18,   70 );  // Baño
  DIMMER cocina ("LT5", "localhost", 1883, 18,   90);   // Cocina quincho

  TEMPERATURE T1("CPUTemp", "localhost", 1883,
                 "/sys/devices/ocp.3/44e10448.bandgap/temp1_input", 60000);

  lighting::time_t scan_time;
  bool bell_sw = false;
  LAPSE lapse;
  do
    {
      scan_time = lapse.get_lapse();

      bell_sw = BELL.getValue();


      // patio, Slot 10.2
      pru_0.set_pwm(GPIO0_27_SP, patio.looop(scan_time,
                                          SW1.getValue(), bell_sw));

      // salon Slot 10.1
      pru_0.set_pwm(GPIO1_15_SP, salon.looop(scan_time,
                                          SW2.getValue(), bell_sw));

      // Ingreso Slot 10.3
      pru_0.set_pwm(GPIO0_22_SP, ingreso.looop(scan_time,
                                            SW3.getValue(), bell_sw));

      // baño Slot 10.4
      pru_0.set_pwm(GPIO1_30_SP, bano.looop(scan_time,
                                         SW4.getValue(), bell_sw));

      // cocina Slot 9.1
      pru_0.set_pwm(GPIO1_4_SP, cocina.looop(scan_time,
                                          SW5.getValue(), bell_sw));


      T1.looop(scan_time);

      std::this_thread::sleep_for(std::chrono::milliseconds(4));

      signal(SIGINT, signal_handler);
      if (finish_signal)
        break;
    }while(true);

  mosqpp::lib_cleanup(); // DIMMERS and TEMPERATURES get destroyed here
  pru_0.~PRULOADER();
  std::cout << "We've cleaned all up" << std::endl;
  return 0;
}
//
// main.cpp ends here

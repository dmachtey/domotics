// Filename: main.cpp
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Mon Jul 25 11:44:00 2016 (-0500)
//
// Last-Updated: Tue Aug  9 16:23:23 2016 (-0500)
//           By: Damian Machtey
//     Update #: 149

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
#include <csignal>

#include "coil.h"
#include "dimmer.h"
#include "domtypes.h"
#include "pru_data.hp"
#include "lapse.h"
#include "temperature.h"
#include "pru_loader.h"
#include "gpio.h"

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
  mosqpp::lib_init();


  // Configure and load pruss software
  PRULOADER pru_0("/usr/lib/domotics/pwm.bin", zero);


  // Inputs declaration
  GPIO SW1(4);   // patio
  GPIO SW2(51);  // salon
  GPIO SW3(50);  // ingreso
  GPIO SW4(60);  // baño
  GPIO SW5(115); // Cocina
  GPIO BELL(14); // Timbre

  SW1.setDirection(INPUT);
  SW2.setDirection(INPUT);
  SW3.setDirection(INPUT);
  SW4.setDirection(INPUT);
  SW5.setDirection(INPUT);
  BELL.setDirection(INPUT);

  // Outputs declaration
  DIMMER patio  ("localhost", 1883, "LT1", 18*4, 100);  // Patio
  DIMMER salon  ("localhost", 1883, "LT2", 18*4, 100);  // Salon quincho
  DIMMER ingreso("localhost", 1883, "LT3", 18,   100);  // Ingreso quincho
  DIMMER bano   ("localhost", 1883, "LT4", 18,   70 );  // Baño
  DIMMER cocina ("localhost", 1883, "LT5", 18,   90);   // Cocina quincho

  TEMPERATURE T1("CPUTemp", "localhost", 1883,
                 "/sys/devices/ocp.3/44e10448.bandgap/temp1_input", 60000);

  lighting::time_t scan_time;
  uint bell_sw;
  LAPSE lapse;
  do
    {
      scan_time = lapse.get_lapse();

      bell_sw = BELL.getValue();


      // patio, Slot 10.2
      pru_0.set_pwm(GPIO0_27, patio.looop(scan_time,
                                          SW1.getValue(), bell_sw));

      // salon Slot 10.1
      pru_0.set_pwm(GPIO1_15, salon.looop(scan_time,
                                          SW2.getValue(), bell_sw));

      // Ingreso Slot 10.3
      pru_0.set_pwm(GPIO0_22, ingreso.looop(scan_time,
                                          SW3.getValue(), bell_sw));

      // baño Slot 10.4
      pru_0.set_pwm(GPIO1_30, bano.looop(scan_time,
                                         SW4.getValue(), bell_sw));

      // cocina Slot 9.1
      pru_0.set_pwm(GPIO1_4, cocina.looop(scan_time,
                                          SW5.getValue(), bell_sw));



      usleep(1000);

      signal(SIGINT, signal_handler);
      if (finish_signal)
        break;
    }while(true);

  mosqpp::lib_cleanup(); // DIMMERS and TEMPERATURES get destroyed here
  pru_0.PRULOADER();
 std::cout << "We've cleaned all up" << std::endl;
  return 0;
}
//
// main.cpp ends here

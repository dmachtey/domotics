// Filename: pru-loader.cpp
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: Fri Aug  5 19:44:08 2016 (-0500)
//
// Last-Updated: Sat Aug  6 02:57:50 2016 (-0500)
//           By: Damian Machtey
//     Update #: 13

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
#include "pru-loader.h"

namespace lighting{

  PRULOADER::PRULOADER(std::string prubinary, pru_num pru){

    if(getuid()!=0){
      std::cerr << "Error on PRULOADER. You must run this program as root. Exiting,,,"
                << std::endl;
        exit(EXIT_FAILURE);


        // Initialize structure used by prussdrv_pruintc_intc
        // PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h
        tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;


        // Allocate and initialize memory
        prussdrv_init ();
        if (pru_num == zero)
          prussdrv_open (PRU_EVTOUT_0);
        else
          prussdrv_open (PRU_EVTOUT_1);


        // Map PRU's INTC
        prussdrv_pruintc_init(&pruss_intc_initdata);

        // place the samples in memory
        if (pru_num == zero)
          prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruDataMemory);
        else
          prussdrv_map_prumem(PRUSS0_PRU1_DATARAM, &pruDataMemory);


        pruDataMemory_int = (unsigned int *) pruDataMemory;


        // Load and execute binary on PRU
        if (prussdrv_exec_program (pru_num, prubinary.c_str())){
          std::cerr << "\nError on PRULOADER: Pru file not loaded " << std::endl;
          exit(EXIT_FAILURE);
        }


        // just in case, clear the exchange memory
        for (int r = 0; r<(256); r++)
          *(pruDataMemory_int+r) = 0;
      }
  }


  void PRULOADER::set_pwm(uint gpio, uint pwm){
    uint local_pwm = pwm;
    if (pwm < 0 ) local_pwm = 0;
    if (pwm > LOOPCOUNTER) local_pwm = LOOPCOUNTER;

    *(pruDataMemory_int+(gpio/4)) = local_pwm;
  }

} // namespace
//
// pru-loader.cpp ends here

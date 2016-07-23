// Filename: test-pwm.cpp
// 
// Description: 
// Author: Damian Machtey
// Maintainer: 
// 
// Created: 2015-06-22 Mon
// 
// Last-Updated: Sat Jul 23 12:44:57 2016 (-0500)
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


#include "dimmer_ctl.h"
#include "temperature.h"
#include <time.h>
#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>

#include <unistd.h> //for usleep, getuid
#include "GPIO.h"
#include <csignal>


#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM 0
static void *pru0DataMemory;
static unsigned int *pru0DataMemory_int;


using namespace lighting;
using namespace std;
using namespace exploringBB;

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

int finish_signal = 0;

void signal_handler(int signal)
{
 finish_signal = 1;
}


int main(int argc, char *argv[])
{
  if(getuid()!=0){
    cerr << "Error on Main. You must run this program as root. Exiting "
         << endl;
    exit(EXIT_FAILURE);
  }

  /* initialize random seed: */
  srand (time(NULL));

  mosqpp::lib_init();

  GPIO SW1(4); //gpio1[28] | P9_12
  GPIO SW2(51);
  GPIO SW3(50);
  GPIO SW4(60);
  GPIO SW5(115); // Cocina quincho
  GPIO BELL(14); // Timbre

  SW1.setDirection(INPUT);
  SW2.setDirection(INPUT);
  SW3.setDirection(INPUT);
  SW4.setDirection(INPUT);
  SW5.setDirection(INPUT);
  BELL.setDirection(INPUT);

  DIMMER LT1((char *)"aut1", (char *)"localhost", 1883, (char *)"LT1"); // Patio
  DIMMER LT2((char *)"aut2", (char *)"localhost", 1883, (char *)"LT2");  // Salon quincho
  DIMMER LT3((char *)"aut3", (char *)"localhost", 1883, (char *)"LT3");  // Ingreso quincho
  DIMMER LT4((char *)"aut4", (char *)"localhost", 1883, (char *)"LT4");  // Baño
  DIMMER LT5((char *)"aut5", (char *)"localhost", 1883, (char *)"LT5"); // Cocina quincho

  TEMPERATURE T1((char *)"cputemp", (char *)"localhost", 1883, (char *)"CPUTemp",
                 (char *)"/sys/devices/ocp.3/44e10448.bandgap/temp1_input", 60000);


  // Initialize structure used by prussdrv_pruintc_intc
  // PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

  // Allocate and initialize memory
  prussdrv_init ();
  prussdrv_open (PRU_EVTOUT_0);

  // Map PRU's INTC
  prussdrv_pruintc_init(&pruss_intc_initdata);

  // place the samples in memory
  prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pru0DataMemory);
  pru0DataMemory_int = (unsigned int *) pru0DataMemory;

  // Load and execute binary on PRU
  if (prussdrv_exec_program (PRU_NUM, "/pru2/pwm.bin")){
    cerr << "\nError on Main: Pru file not loaded " << endl;
    exit(EXIT_FAILURE);
  }


  // just in case, clear the exchange memory
  for (int r = 0; r<32; r++)
    *(pru0DataMemory_int+r) = 0;


  unsigned int previus_scan;
  timespec prevt, actualt;
  clock_gettime(CLOCK_MONOTONIC, &actualt);
  prevt = actualt;
  do{

    clock_gettime(CLOCK_MONOTONIC, &actualt);
    previus_scan = diff(prevt, actualt);
    prevt = actualt;


    unsigned int bell = BELL.getValue();

    LT1.dloop(SW1.getValue(), previus_scan, bell); // Patio
    LT2.dloop(SW2.getValue(), previus_scan, bell); // Salon quincho
    LT3.dloop(SW3.getValue(), previus_scan, bell); // Ingreso quincho
    LT4.dloop(SW4.getValue(), previus_scan, bell); // Baño quincho
    LT5.dloop(SW5.getValue(), previus_scan, bell); // Cocina quincho


    //    cout << SW1.getValue() << endl;

    // SLOT 10 {47, 27, 22, 62}
    // GPIO 27
    unsigned int LTDuty = LT1.getDuty(); // Patio quincho
    *(pru0DataMemory_int+5) = LTDuty; //P8_17

    // GPIO 47
    LTDuty = LT2.getDuty();          // Salon quincho
    *(pru0DataMemory_int+13) = LTDuty;

    // GPIO 22
    LTDuty = LT3.getDuty();  // Ingreso quincho
    *(pru0DataMemory_int+2) = LTDuty;

    // GPIO 62
    LTDuty = LT4.getDuty(); // Baño quincho
    *(pru0DataMemory_int+17) = LTDuty;

    // SLOT 9 {36, 32, 86, 87}
    // GPIO 36  Cocina Quincho
    LTDuty = LT5.getDuty();
    *(pru0DataMemory_int+19) = LTDuty;

    T1.tloop(previus_scan);


    usleep(50);      // sleep for ten millisecond
    signal(SIGINT, signal_handler);
    if (finish_signal)
      break;
  }while(1);



  // Cleanup every thing
  mosqpp::lib_cleanup(); // DIMMERS and TEMPERATURES get destroyed here
  cout << "We've cleaned up all" << endl;
  return 0;
}

// 
// test-pwm.cpp ends here

// Filename: pwm.p -*- mode: asm -*-
//
// Description:
// Author: Damian Machtey
// Maintainer:
//
// Created: 2015-06-22 Mon
//
// Last-Updated: Fri Jun 30 17:14:01 2017 (-0300)
//           By: Damian Machtey
//     Update #: 11

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
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
// Note: to compile: pasm -b pwm.p
// Code:


.origin 0               // offset of start of program in PRU memory
.entrypoint START       // program entry point used by the debugger

#include "pru_data.hp"
#include "pwm_macros.hp"

START:
	// Enable the OCP master port
	LBCO    r0, C4, 4, 4     // load SYSCFG reg into r0 (use c4 const addr)
	CLR     r0, r0, 4        // clear bit 4 (STANDBY_INIT)
	SBCO    r0, C4, 4, 4     // store the modified r0 back at the load addr

    MOV      r29, 0 // r29=0

MAINLOOP:
    DELAY

 // PWM_MAIN    gpio,     remote SP   | 0x100, bank[0..3]
    PWM_MAIN    GPIO0_27, GPIO0_27_SP | 0x100, GPIO0 // Patio quincho
    PWM_MAIN    GPIO1_15, GPIO1_15_SP | 0x100, GPIO1 // Salon quincho
    PWM_MAIN    GPIO0_22, GPIO0_22_SP | 0x100, GPIO0 // Ingreso quincho
    PWM_MAIN    GPIO1_30, GPIO1_30_SP | 0x100, GPIO1 // Baño quincho
    PWM_MAIN    GPIO1_4,  GPIO1_4_SP  | 0x100, GPIO1 // Cocina quincho



    ADD	r29, r29, 1        // increment REG29 by 1
    QBGE MAINLOOP, r29, LOOPCOUNTER  // if LOOPCOUNTER >= r29 jump (NO RESET R29)

    CPY_SP                 // copy remote setpoints to locals
    MOV	r29, 0             // reset r29
    QBA MAINLOOP           // loop forever

END:                               // end of program, send back interrupt
	MOV	R31.b0, PRU0_R31_VEC_VALID | EVENTOUT0
	HALT

//
// pwm.p ends here

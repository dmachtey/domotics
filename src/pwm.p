// PRUSS program to output a simple PWM signal at variable rate
// Writen by Damian Machtey 2015-06-22 Mon
// to compile this; pasm -b pwm.p




.origin 0               // offset of start of program in PRU memory
.entrypoint START       // program entry point used by the debugger

#define GPIO0 0x44e07000         // GPIO Bank 0, See the AM335x TRM
#define GPIO1 0x4804c000         // GPIO Bank 1, Table 2.2 Peripheral Map
#define GPIO2 0x481ac000         // GPIO Bank 2,
#define GPIO3 0x481ae000         // GPIO Bank 3,

#define LOOPCOUNTER  100         // Length of counter, 100 up 0 down/LEVELS

#define GPIO_CLEARDATAOUT 0x190  // for clearing the GPIO registers
#define GPIO_SETDATAOUT   0x194  // for setting the GPIO registers
#define GPIO_DATAIN       0x138  // to read the register data read from GPIO pins

//#define GPIO0_30 1<<30           // P9_11 gpio0[30] Output - bit 30


#define GPIO0_10 1<<10 //   0x0D8 | gpio0[10] | P8_31 | 10 |  0
#define GPIO0_11 1<<11 //   0x0DC | gpio0[11] | P8_32 | 11 |  1
#define GPIO0_22 1<<22 //   0x020 | gpio0[22] | P8_19 | 22 |  2
#define GPIO0_23 1<<23 //   0x024 | gpio0[23] | P8_13 | 23 |  3
#define GPIO0_26 1<<26 //   0x028 | gpio0[26] | P8_14 | 26 |  4
#define GPIO0_27 1<<27 //   0x02C | gpio0[27] | P8_17 | 27 |  5
#define GPIO0_8  1<<8  //   0x0D0 | gpio0[8]  | P8_35 |  8 |  6
#define GPIO0_9  1<<0  //   0x0D4 | gpio0[9]  | P8_33 |  9 |  7
#define GPIO1_0  1<<0  //   0x000 | gpio1[0]  | P8_25 | 32 |  8
#define GPIO1_1  1<<1  //   0x004 | gpio1[1]  | P8_24 | 33 |  9
#define GPIO1_12 1<<12 //   0x030 | gpio1[12] | P8_12 | 44 | 10
#define GPIO1_13 1<<13 //   0x034 | gpio1[13] | P8_11 | 45 | 11
#define GPIO1_14 1<<14 //   0x038 | gpio1[14] | P8_16 | 46 | 12
#define GPIO1_15 1<<15 //   0x03C | gpio1[15] | P8_15 | 47 | 13
#define GPIO1_2  1<<2  //   0x008 | gpio1[2]  | P8_05 | 34 | 14
#define GPIO1_29 1<<29 //   0x07C | gpio1[29] | P8_26 | 61 | 15
#define GPIO1_3  1<<3  //   0x00C | gpio1[3]  | P8_06 | 35 | 16
#define GPIO1_30 1<<30 //   0x080 | gpio1[30] | P8_21 | 62 | 17
#define GPIO1_31 1<<31 //   0x084 | gpio1[31] | P8_20 | 63 | 18
#define GPIO1_4  1<<4  //   0x010 | gpio1[4]  | P8_23 | 36 | 19
#define GPIO1_5  1<<5  //   0x014 | gpio1[5]  | P8_22 | 37 | 20
#define GPIO1_6  1<<6  //   0x018 | gpio1[6]  | P8_03 | 38 | 21
#define GPIO1_7  1<<7  //   0x01C | gpio1[7]  | P8_04 | 39 | 22
#define GPIO2_1  1<<1  //   0x08C | gpio2[1]  | P8_18 | 65 | 23
#define GPIO2_17 1<<17 //   0x0CC | gpio2[17] | P8_34 | 81 | 24
#define GPIO2_2  1<<2  //   0x090 | gpio2[2]  | P8_07 | 66 | 25
#define GPIO2_22 1<<22 //   0x0E0 | gpio2[22] | P8_27 | 86 | 26
#define GPIO2_23 1<<23 //   0x0E4 | gpio2[23] | P8_29 | 87 | 27
#define GPIO2_24 1<<24 //   0x0E8 | gpio2[24] | P8_28 | 88 | 28
#define GPIO2_25 1<<25 //   0x0EC | gpio2[25] | P8_30 | 89 | 29
#define GPIO2_3  1<<3  //   0x094 | gpio2[3]  | P8_08 | 67 | 30
#define GPIO2_4  1<<4  //   0x098 | gpio2[4]  | P8_10 | 68 | 31
#define GPIO2_5  1<<5  //   0x09C | gpio2[5]  | P8_09 | 69 | 32

// Set Point Address
#define GPIO0_10_SP 0x00000000   //   0x0D8 | gpio0[10] | P8_31 | 10 |  0
#define GPIO0_11_SP 0x00000004   //   0x0DC | gpio0[11] | P8_32 | 11 |  1
#define GPIO0_22_SP 0x00000008   //   0x020 | gpio0[22] | P8_19 | 22 |  2
#define GPIO0_23_SP 0x0000000c   //   0x024 | gpio0[23] | P8_13 | 23 |  3
#define GPIO0_26_SP 0x00000010   //   0x028 | gpio0[26] | P8_14 | 26 |  4
#define GPIO0_27_SP 0x00000014   //   0x02C | gpio0[27] | P8_17 | 27 |  5
#define GPIO0_8_SP  0x00000018   //   0x0D0 | gpio0[8]  | P8_35 |  8 |  6
#define GPIO0_9_SP  0x0000001c   //   0x0D4 | gpio0[9]  | P8_33 |  9 |  7
#define GPIO1_0_SP  0x00000020   //   0x000 | gpio1[0]  | P8_25 | 32 |  8
#define GPIO1_1_SP  0x00000024   //   0x004 | gpio1[1]  | P8_24 | 33 |  9
#define GPIO1_12_SP 0x00000028   //   0x030 | gpio1[12] | P8_12 | 44 | 10
#define GPIO1_13_SP 0x0000002c   //   0x034 | gpio1[13] | P8_11 | 45 | 11
#define GPIO1_14_SP 0x00000030   //   0x038 | gpio1[14] | P8_16 | 46 | 12
#define GPIO1_15_SP 0x00000034   //   0x03C | gpio1[15] | P8_15 | 47 | 13
#define GPIO1_2_SP  0x00000038   //   0x008 | gpio1[2]  | P8_05 | 34 | 14
#define GPIO1_29_SP 0x0000003c   //   0x07C | gpio1[29] | P8_26 | 61 | 15
#define GPIO1_3_SP  0x00000040   //   0x00C | gpio1[3]  | P8_06 | 35 | 16
#define GPIO1_30_SP 0x00000044   //   0x080 | gpio1[30] | P8_21 | 62 | 17
#define GPIO1_31_SP 0x00000048   //   0x084 | gpio1[31] | P8_20 | 63 | 18
#define GPIO1_4_SP  0x0000004c   //   0x010 | gpio1[4]  | P8_23 | 36 | 19
#define GPIO1_5_SP  0x00000050   //   0x014 | gpio1[5]  | P8_22 | 37 | 20
#define GPIO1_6_SP  0x00000054   //   0x018 | gpio1[6]  | P8_03 | 38 | 21
#define GPIO1_7_SP  0x00000058   //   0x01C | gpio1[7]  | P8_04 | 39 | 22
#define GPIO2_1_SP  0x0000005c   //   0x08C | gpio2[1]  | P8_18 | 65 | 23
#define GPIO2_17_SP 0x00000060   //   0x0CC | gpio2[17] | P8_34 | 81 | 24
#define GPIO2_2_SP  0x00000064   //   0x090 | gpio2[2]  | P8_07 | 66 | 25
#define GPIO2_22_SP 0x00000068   //   0x0E0 | gpio2[22] | P8_27 | 86 | 26
#define GPIO2_23_SP 0x0000006c   //   0x0E4 | gpio2[23] | P8_29 | 87 | 27
#define GPIO2_24_SP 0x00000070   //   0x0E8 | gpio2[24] | P8_28 | 88 | 28
#define GPIO2_25_SP 0x00000074   //   0x0EC | gpio2[25] | P8_30 | 89 | 29
#define GPIO2_3_SP  0x00000078   //   0x094 | gpio2[3]  | P8_08 | 67 | 30
#define GPIO2_4_SP  0x0000007c   //   0x098 | gpio2[4]  | P8_10 | 68 | 31
#define GPIO2_5_SP  0x00000080   //   0x09C | gpio2[5]  | P8_09 | 69 | 32


#define INS_PER_US   200         // 5ns per instruction
#define INS_PER_DELAY_LOOP 2     // two instructions per delay loop

#define INS_PER_GPIO 12

//
// At frequencies above 1250 Hz, no restrictions on the percent
//  flicker. (Note: this is the minimum allowable frequency for basic
//  pulse-width modulation (PWM)-based dimming.)
//
#define TARGET_HZ 777
#define N_GPIOS 33
#define TARGET_LOOP_TIME (1000000 / TARGET_HZ)  // us that shuld last each main loop
#define INS_OVERHEAD 4


#define DELAY_T_USED ((INS_PER_GPIO * N_GPIOS + INS_OVERHEAD) * LOOPCOUNTER)/INS_PER_US // time us consumed by program
#define DELAY_T  (TARGET_LOOP_TIME - DELAY_T_USED)
#define DELAY_INS ((DELAY_T * INS_PER_US) / (LOOPCOUNTER * INS_PER_DELAY_LOOP))

#define PRU0_R31_VEC_VALID 32
#define EVENTOUT0 3




// MACROS
//GPIO0_10 start
.macro GPIO0_10ON
	MOV	r2, GPIO0 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO0_10     // write GPIO0_10 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO0_10OFF
	MOV	r2, GPIO0 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO0_10     // write GPIO0_10 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO0_10M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO0_10_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO0_10CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO0_10CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO0_10ON
GPIO0_10CLR:

    QBEQ GPIO0_10END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO0_10END, r1, r29 //  jump if r1<>r29
    GPIO0_10OFF
GPIO0_10END:
.endm
//GPIO0_10 end

//GPIO0_11 start
.macro GPIO0_11ON
	MOV	r2, GPIO0 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO0_11     // write GPIO0_11 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO0_11OFF
	MOV	r2, GPIO0 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO0_11     // write GPIO0_11 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO0_11M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO0_11_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO0_11CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO0_11CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO0_11ON
GPIO0_11CLR:

    QBEQ GPIO0_11END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO0_11END, r1, r29 //  jump if r1<>r29
    GPIO0_11OFF
GPIO0_11END:
.endm
//GPIO0_11 end

//GPIO0_22 start
.macro GPIO0_22ON
	MOV	r2, GPIO0 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO0_22     // write GPIO0_22 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO0_22OFF
	MOV	r2, GPIO0 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO0_22     // write GPIO0_22 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO0_22M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO0_22_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO0_22CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO0_22CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO0_22ON
GPIO0_22CLR:

    QBEQ GPIO0_22END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO0_22END, r1, r29 //  jump if r1<>r29
    GPIO0_22OFF
GPIO0_22END:
.endm
//GPIO0_22 end


//GPIO0_23 start
.macro GPIO0_23ON
	MOV	r2, GPIO0 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO0_23     // write GPIO0_23 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO0_23OFF
	MOV	r2, GPIO0 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO0_23     // write GPIO0_23 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO0_23M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO0_23_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO0_23CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO0_23CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO0_23ON
GPIO0_23CLR:

    QBEQ GPIO0_23END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO0_23END, r1, r29 //  jump if r1<>r29
    GPIO0_23OFF
GPIO0_23END:
.endm
//GPIO0_23 end

//GPIO0_26 start
.macro GPIO0_26ON
	MOV	r2, GPIO0 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO0_26     // write GPIO0_26 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO0_26OFF
	MOV	r2, GPIO0 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO0_26     // write GPIO0_26 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO0_26M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO0_26_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO0_26CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO0_26CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO0_26ON
GPIO0_26CLR:

    QBEQ GPIO0_26END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO0_26END, r1, r29 //  jump if r1<>r29
    GPIO0_26OFF
GPIO0_26END:
.endm
//GPIO0_26 end

//GPIO0_27 start
.macro GPIO0_27ON
	MOV	r2, GPIO0 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO0_27     // write GPIO0_27 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO0_27OFF
	MOV	r2, GPIO0 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO0_27     // write GPIO0_27 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO0_27M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO0_27_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO0_27CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO0_27CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO0_27ON
GPIO0_27CLR:

    QBEQ GPIO0_27END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO0_27END, r1, r29 //  jump if r1<>r29
    GPIO0_27OFF
GPIO0_27END:
.endm
//GPIO0_27 end

//GPIO0_8 start
.macro GPIO0_8ON
	MOV	r2, GPIO0 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO0_8     // write GPIO0_8 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO0_8OFF
	MOV	r2, GPIO0 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO0_8     // write GPIO0_8 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO0_8M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO0_8_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO0_8CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO0_8CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO0_8ON
GPIO0_8CLR:

    QBEQ GPIO0_8END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO0_8END, r1, r29 //  jump if r1<>r29
    GPIO0_8OFF
GPIO0_8END:
.endm
//GPIO0_8 end

//GPIO0_9 start
.macro GPIO0_9ON
	MOV	r2, GPIO0 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO0_9     // write GPIO0_9 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO0_9OFF
	MOV	r2, GPIO0 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO0_9     // write GPIO0_9 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO0_9M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO0_9_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO0_9CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO0_9CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO0_9ON
GPIO0_9CLR:

    QBEQ GPIO0_9END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO0_9END, r1, r29 //  jump if r1<>r29
    GPIO0_9OFF
GPIO0_9END:
.endm
//GPIO0_9 end

//GPIO1_0 start
.macro GPIO1_0ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_0     // write GPIO1_0 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_0OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_0     // write GPIO1_0 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_0M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_0_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_0CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_0CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_0ON
GPIO1_0CLR:

    QBEQ GPIO1_0END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_0END, r1, r29 //  jump if r1<>r29
    GPIO1_0OFF
GPIO1_0END:
.endm
//GPIO1_0 end

//GPIO1_1 start
.macro GPIO1_1ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_1     // write GPIO1_1 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_1OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_1     // write GPIO1_1 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_1M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_1_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_1CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_1CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_1ON
GPIO1_1CLR:

    QBEQ GPIO1_1END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_1END, r1, r29 //  jump if r1<>r29
    GPIO1_1OFF
GPIO1_1END:
.endm
//GPIO1_1 end

//GPIO1_12 start
.macro GPIO1_12ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_12     // write GPIO1_12 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_12OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_12     // write GPIO1_12 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_12M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_12_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_12CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_12CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_12ON
GPIO1_12CLR:

    QBEQ GPIO1_12END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_12END, r1, r29 //  jump if r1<>r29
    GPIO1_12OFF
GPIO1_12END:
.endm
//GPIO1_12 end

//GPIO1_13 start
.macro GPIO1_13ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_13     // write GPIO1_13 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_13OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_13     // write GPIO1_13 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_13M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_13_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_13CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_13CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_13ON
GPIO1_13CLR:

    QBEQ GPIO1_13END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_13END, r1, r29 //  jump if r1<>r29
    GPIO1_13OFF
GPIO1_13END:
.endm
//GPIO1_13 end

//GPIO1_14 start
.macro GPIO1_14ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_14     // write GPIO1_14 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_14OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_14     // write GPIO1_14 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_14M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_14_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_14CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_14CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_14ON
GPIO1_14CLR:

    QBEQ GPIO1_14END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_14END, r1, r29 //  jump if r1<>r29
    GPIO1_14OFF
GPIO1_14END:
.endm
//GPIO1_14 end

//GPIO1_15 start
.macro GPIO1_15ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_15     // write GPIO1_15 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_15OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_15     // write GPIO1_15 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_15M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_15_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_15CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_15CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_15ON
GPIO1_15CLR:

    QBEQ GPIO1_15END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_15END, r1, r29 //  jump if r1<>r29
    GPIO1_15OFF
GPIO1_15END:
.endm
//GPIO1_15 end

//GPIO1_2 start
.macro GPIO1_2ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_2     // write GPIO1_2 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_2OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_2     // write GPIO1_2 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_2M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_2_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_2CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_2CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_2ON
GPIO1_2CLR:

    QBEQ GPIO1_2END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_2END, r1, r29 //  jump if r1<>r29
    GPIO1_2OFF
GPIO1_2END:
.endm
//GPIO1_2 end

//GPIO1_29 start
.macro GPIO1_29ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_29     // write GPIO1_29 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_29OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_29     // write GPIO1_29 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_29M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_29_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_29CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_29CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_29ON
GPIO1_29CLR:

    QBEQ GPIO1_29END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_29END, r1, r29 //  jump if r1<>r29
    GPIO1_29OFF
GPIO1_29END:
.endm
//GPIO1_29 end

//GPIO1_3 start
.macro GPIO1_3ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_3     // write GPIO1_3 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_3OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_3     // write GPIO1_3 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_3M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_3_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_3CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_3CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_3ON
GPIO1_3CLR:

    QBEQ GPIO1_3END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_3END, r1, r29 //  jump if r1<>r29
    GPIO1_3OFF
GPIO1_3END:
.endm
//GPIO1_3 end

//GPIO1_30 start
.macro GPIO1_30ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_30     // write GPIO1_30 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_30OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_30     // write GPIO1_30 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_30M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_30_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_30CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_30CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_30ON
GPIO1_30CLR:

    QBEQ GPIO1_30END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_30END, r1, r29 //  jump if r1<>r29
    GPIO1_30OFF
GPIO1_30END:
.endm
//GPIO1_30 end

//GPIO1_31 start
.macro GPIO1_31ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_31     // write GPIO1_31 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_31OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_31     // write GPIO1_31 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_31M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_31_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_31CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_31CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_31ON
GPIO1_31CLR:

    QBEQ GPIO1_31END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_31END, r1, r29 //  jump if r1<>r29
    GPIO1_31OFF
GPIO1_31END:
.endm
//GPIO1_31 end

//GPIO1_4 start
.macro GPIO1_4ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_4     // write GPIO1_4 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_4OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_4     // write GPIO1_4 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_4M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_4_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_4CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_4CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_4ON
GPIO1_4CLR:

    QBEQ GPIO1_4END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_4END, r1, r29 //  jump if r1<>r29
    GPIO1_4OFF
GPIO1_4END:
.endm
//GPIO1_4 end

//GPIO1_5 start
.macro GPIO1_5ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_5     // write GPIO1_5 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_5OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_5     // write GPIO1_5 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_5M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_5_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_5CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_5CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_5ON
GPIO1_5CLR:

    QBEQ GPIO1_5END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_5END, r1, r29 //  jump if r1<>r29
    GPIO1_5OFF
GPIO1_5END:
.endm
//GPIO1_5 end

//GPIO1_6 start
.macro GPIO1_6ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_6     // write GPIO1_6 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_6OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_6     // write GPIO1_6 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_6M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_6_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_6CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_6CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_6ON
GPIO1_6CLR:

    QBEQ GPIO1_6END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_6END, r1, r29 //  jump if r1<>r29
    GPIO1_6OFF
GPIO1_6END:
.endm
//GPIO1_6 end

//GPIO1_7 start
.macro GPIO1_7ON
	MOV	r2, GPIO1 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO1_7     // write GPIO1_7 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO1_7OFF
	MOV	r2, GPIO1 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO1_7     // write GPIO1_7 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO1_7M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO1_7_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO1_7CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO1_7CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO1_7ON
GPIO1_7CLR:

    QBEQ GPIO1_7END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO1_7END, r1, r29 //  jump if r1<>r29
    GPIO1_7OFF
GPIO1_7END:
.endm
//GPIO1_7 end

//GPIO2_1 start
.macro GPIO2_1ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_1     // write GPIO2_1 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_1OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_1     // write GPIO2_1 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_1M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_1_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_1CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_1CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_1ON
GPIO2_1CLR:

    QBEQ GPIO2_1END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_1END, r1, r29 //  jump if r1<>r29
    GPIO2_1OFF
GPIO2_1END:
.endm
//GPIO2_1 end

//GPIO2_17 start
.macro GPIO2_17ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_17     // write GPIO2_17 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_17OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_17     // write GPIO2_17 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_17M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_17_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_17CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_17CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_17ON
GPIO2_17CLR:

    QBEQ GPIO2_17END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_17END, r1, r29 //  jump if r1<>r29
    GPIO2_17OFF
GPIO2_17END:
.endm
//GPIO2_17 end

//GPIO2_2 start
.macro GPIO2_2ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_2     // write GPIO2_2 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_2OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_2     // write GPIO2_2 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_2M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_2_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_2CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_2CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_2ON
GPIO2_2CLR:

    QBEQ GPIO2_2END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_2END, r1, r29 //  jump if r1<>r29
    GPIO2_2OFF
GPIO2_2END:
.endm
//GPIO2_2 end

//GPIO2_22 start
.macro GPIO2_22ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_22     // write GPIO2_22 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_22OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_22     // write GPIO2_22 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_22M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_22_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_22CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_22CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_22ON
GPIO2_22CLR:

    QBEQ GPIO2_22END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_22END, r1, r29 //  jump if r1<>r29
    GPIO2_22OFF
GPIO2_22END:
.endm
//GPIO2_22 end

//GPIO2_23 start
.macro GPIO2_23ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_23     // write GPIO2_23 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_23OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_23     // write GPIO2_23 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_23M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_23_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_23CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_23CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_23ON
GPIO2_23CLR:

    QBEQ GPIO2_23END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_23END, r1, r29 //  jump if r1<>r29
    GPIO2_23OFF
GPIO2_23END:
.endm
//GPIO2_23 end

//GPIO2_24 start
.macro GPIO2_24ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_24     // write GPIO2_24 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_24OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_24     // write GPIO2_24 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_24M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_24_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_24CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_24CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_24ON
GPIO2_24CLR:

    QBEQ GPIO2_24END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_24END, r1, r29 //  jump if r1<>r29
    GPIO2_24OFF
GPIO2_24END:
.endm
//GPIO2_24 end

//GPIO2_25 start
.macro GPIO2_25ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_25     // write GPIO2_25 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_25OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_25     // write GPIO2_25 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_25M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_25_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_25CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_25CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_25ON
GPIO2_25CLR:

    QBEQ GPIO2_25END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_25END, r1, r29 //  jump if r1<>r29
    GPIO2_25OFF
GPIO2_25END:
.endm
//GPIO2_25 end

//GPIO2_3 start
.macro GPIO2_3ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_3     // write GPIO2_3 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_3OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_3     // write GPIO2_3 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_3M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_3_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_3CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_3CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_3ON
GPIO2_3CLR:

    QBEQ GPIO2_3END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_3END, r1, r29 //  jump if r1<>r29
    GPIO2_3OFF
GPIO2_3END:
.endm
//GPIO2_3 end

//GPIO2_4 start
.macro GPIO2_4ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_4     // write GPIO2_4 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_4OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_4     // write GPIO2_4 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_4M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_4_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_4CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_4CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_4ON
GPIO2_4CLR:

    QBEQ GPIO2_4END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_4END, r1, r29 //  jump if r1<>r29
    GPIO2_4OFF
GPIO2_4END:
.endm
//GPIO2_4 end

//GPIO2_5 start
.macro GPIO2_5ON
	MOV	r2, GPIO2 | GPIO_SETDATAOUT  // load addr for GPIO Set data  r2
	MOV	r3, GPIO2_5     // write GPIO2_5 to r3
    SBBO r3, r2, 0, 4     // write r3 to the r2 address value - LED ON
.endm

.macro GPIO2_5OFF
	MOV	r2, GPIO2 | GPIO_CLEARDATAOUT // load addr for GPIO Clear data
	MOV	r3, GPIO2_5     // write GPIO2_5 to r3
	SBBO r3, r2, 0, 4     // write r3 to the r2 address - LED OFF
.endm

.macro GPIO2_5M
    // load the set point into a register r1
    // Reading the memory that was set by the C program into registers
    // r2 - Load the sample time delay
    MOV	r0, GPIO2_5_SP	   //load the memory location
    LBBO r1, r0, 0, 4	   //load the set point into r1

    QBEQ GPIO2_5CLR, r1, 0    //  if set point == 0, jump, KEEPT IT OFF
    QBNE GPIO2_5CLR, r29, 0   //  if counter <> 0, jump
                               //  else turn it on
    GPIO2_5ON
GPIO2_5CLR:

    QBEQ GPIO2_5END, r1, LOOPCOUNTER //  jump if r1 (Set point)==LOOPCOUNTER (allways on)
    QBNE GPIO2_5END, r1, r29 //  jump if r1<>r29
    GPIO2_5OFF
GPIO2_5END:
.endm
//GPIO2_5 end

.macro DELAY
    mov r4, DELAY_INS  // load r4 DELAY
LOOPDELAY:
    sub r4, r4, 1
    qbne LOOPDELAY, r4, 0
.endm
// END MACROS


START:
	// Enable the OCP master port
	LBCO    r0, C4, 4, 4     // load SYSCFG reg into r0 (use c4 const addr)
	CLR     r0, r0, 4        // clear bit 4 (STANDBY_INIT)
	SBCO    r0, C4, 4, 4     // store the modified r0 back at the load addr

    MOV      r29, 0 // r29=0

MAINLOOP:
    DELAY
    GPIO0_10M
	GPIO0_11M
	GPIO0_22M
	GPIO0_23M
	GPIO0_26M
	GPIO0_27M
	GPIO0_8M
	GPIO0_9M
	GPIO1_0M
	GPIO1_1M
	GPIO1_12M
	GPIO1_13M
	GPIO1_14M
	GPIO1_15M
	GPIO1_2M
	GPIO1_29M
	GPIO1_3M
	GPIO1_30M
	GPIO1_31M
	GPIO1_4M
	GPIO1_5M
	GPIO1_6M
	GPIO1_7M
	GPIO2_1M
	GPIO2_17M
	GPIO2_2M
	GPIO2_22M
	GPIO2_23M
	GPIO2_24M
	GPIO2_25M
	GPIO2_3M
	GPIO2_4M
	GPIO2_5M


    ADD	r29, r29, 1        // increment REG29 by 1
    QBGE MAINLOOP, r29, LOOPCOUNTER  // if LOOPCOUNTER >= r29 jump (NO RESET R29)

    MOV	r29, 0             // reset r29
    QBA MAINLOOP           // loop forever

END:                               // end of program, send back interrupt
	MOV	R31.b0, PRU0_R31_VEC_VALID | EVENTOUT0
	HALT

EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "noname.sch"
Date "17 jul 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PC817 IC1
U 1 1 55A81D51
P 4500 1100
F 0 "IC1" H 4290 1290 40  0000 C CNN
F 1 "PC817" H 4680 910 40  0000 C CNN
F 2 "Housings_DIP:DIP-4_W7.62mm_LongPads" H 4290 920 30  0000 C CIN
F 3 "~" H 4500 1100 60  0000 C CNN
	1    4500 1100
	-1   0    0    1   
$EndComp
$Comp
L PC817 IC3
U 1 1 55BF39C5
P 4500 2700
F 0 "IC3" H 4290 2890 40  0000 C CNN
F 1 "PC817" H 4680 2510 40  0000 C CNN
F 2 "Housings_DIP:DIP-4_W7.62mm_LongPads" H 4290 2520 30  0000 C CIN
F 3 "~" H 4500 2700 60  0000 C CNN
	1    4500 2700
	-1   0    0    1   
$EndComp
Text Label 4200 2800 2    60   ~ 0
+12V
Text Label 4800 1000 0    60   ~ 0
GND
Text Label 4800 2600 0    60   ~ 0
GND
$Comp
L PC817 IC2
U 1 1 55BF39C8
P 4500 1850
F 0 "IC2" H 4290 2040 40  0000 C CNN
F 1 "PC817" H 4680 1660 40  0000 C CNN
F 2 "Housings_DIP:DIP-4_W7.62mm_LongPads" H 4290 1670 30  0000 C CIN
F 3 "~" H 4500 1850 60  0000 C CNN
	1    4500 1850
	-1   0    0    1   
$EndComp
$Comp
L PC817 IC4
U 1 1 55B15546
P 4500 3450
F 0 "IC4" H 4290 3640 40  0000 C CNN
F 1 "PC817" H 4680 3260 40  0000 C CNN
F 2 "Housings_DIP:DIP-4_W7.62mm_LongPads" H 4290 3270 30  0000 C CIN
F 3 "~" H 4500 3450 60  0000 C CNN
	1    4500 3450
	-1   0    0    1   
$EndComp
Text Label 4200 1950 2    60   ~ 0
+12V
Text Label 4200 3550 2    60   ~ 0
+12V
Text Label 4800 1750 0    60   ~ 0
GND
Text Label 4800 3350 0    60   ~ 0
GND
Text Label 4850 1200 0    60   ~ 0
in_1
Text Label 4850 1950 0    60   ~ 0
in_2
Text Label 4850 2800 0    60   ~ 0
in_3
Text Label 4850 3550 0    60   ~ 0
in_4
$Comp
L CONN_02X10 P1
U 1 1 55BF4604
P 950 5800
F 0 "P1" H 950 6350 50  0000 C CNN
F 1 "CONN_02X10" V 950 5800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_2x10" H 950 4600 60  0000 C CNN
F 3 "" H 950 4600 60  0000 C CNN
	1    950  5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 1200 4800 1200
Wire Wire Line
	4850 2800 4800 2800
Wire Wire Line
	4850 1950 4800 1950
Wire Wire Line
	4850 3550 4800 3550
Text Label 4200 1200 2    60   ~ 0
+12V
Wire Wire Line
	4200 1500 4200 1200
Wire Wire Line
	4200 2250 4200 1950
Wire Wire Line
	4200 3100 4200 2800
Wire Wire Line
	4200 3850 4200 3550
Wire Wire Line
	700  5350 1200 5350
Wire Wire Line
	700  5450 1200 5450
Wire Wire Line
	700  5550 1200 5550
Wire Wire Line
	700  5650 1200 5650
Wire Wire Line
	700  5750 1200 5750
Wire Wire Line
	700  5850 1200 5850
Wire Wire Line
	700  5950 1200 5950
Wire Wire Line
	700  6050 1200 6050
Wire Wire Line
	700  6150 1200 6150
Wire Wire Line
	700  6250 1200 6250
Text Label 1200 6250 0    60   ~ 0
in_1
Text Label 1200 6150 0    60   ~ 0
in_2
Text Label 1200 6050 0    60   ~ 0
in_3
Text Label 1200 5950 0    60   ~ 0
in_4
Text Label 1200 5350 0    60   ~ 0
GND
Text Label 1200 5450 0    60   ~ 0
3.3V
Text Label 1200 5550 0    60   ~ 0
GND12V
Text Label 1200 5650 0    60   ~ 0
+12VDC
$EndSCHEMATC

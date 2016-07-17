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
LIBS:in-board-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Plugable Input Board"
Date "2015-08-04"
Rev ""
Comp "Damian Machtey"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PC817 IC1
U 1 1 55A81D51
P 6450 3250
F 0 "IC1" H 6240 3440 40  0000 C CNN
F 1 "PC817" H 6630 3060 40  0000 C CNN
F 2 "Housings_DIP:DIP-4_W7.62mm_LongPads" H 6240 3070 30  0000 C CIN
F 3 "~" H 6450 3250 60  0000 C CNN
	1    6450 3250
	-1   0    0    1   
$EndComp
$Comp
L PC817 IC3
U 1 1 55BF39C5
P 6450 4250
F 0 "IC3" H 6240 4440 40  0000 C CNN
F 1 "PC817" H 6630 4060 40  0000 C CNN
F 2 "Housings_DIP:DIP-4_W7.62mm_LongPads" H 6240 4070 30  0000 C CIN
F 3 "~" H 6450 4250 60  0000 C CNN
	1    6450 4250
	-1   0    0    1   
$EndComp
Text Label 6750 3150 0    60   ~ 0
GND12V
$Comp
L PC817 IC2
U 1 1 55BF39C8
P 6450 3750
F 0 "IC2" H 6240 3940 40  0000 C CNN
F 1 "PC817" H 6630 3560 40  0000 C CNN
F 2 "Housings_DIP:DIP-4_W7.62mm_LongPads" H 6240 3570 30  0000 C CIN
F 3 "~" H 6450 3750 60  0000 C CNN
	1    6450 3750
	-1   0    0    1   
$EndComp
$Comp
L PC817 IC4
U 1 1 55B15546
P 6450 4750
F 0 "IC4" H 6240 4940 40  0000 C CNN
F 1 "PC817" H 6630 4560 40  0000 C CNN
F 2 "Housings_DIP:DIP-4_W7.62mm_LongPads" H 6240 4570 30  0000 C CIN
F 3 "~" H 6450 4750 60  0000 C CNN
	1    6450 4750
	-1   0    0    1   
$EndComp
Text Label 7200 3350 0    60   ~ 0
in_1
Text Label 7200 3850 0    60   ~ 0
in_2
Text Label 7200 4350 0    60   ~ 0
in_3
Text Label 7200 4850 0    60   ~ 0
in_4
$Comp
L CONN_02X10 P1
U 1 1 55BF4604
P 4150 4050
F 0 "P1" H 4150 4600 50  0000 C CNN
F 1 "CONN_02X10" V 4150 4050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_2x10" H 4150 2850 60  0000 C CNN
F 3 "" H 4150 2850 60  0000 C CNN
	1    4150 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3350 6900 3350
Wire Wire Line
	6750 4350 6900 4350
Wire Wire Line
	6750 3850 6900 3850
Wire Wire Line
	6750 4850 6900 4850
Wire Wire Line
	3900 3600 4400 3600
Wire Wire Line
	3900 3700 4400 3700
Wire Wire Line
	3900 3800 4400 3800
Wire Wire Line
	3900 3900 4400 3900
Wire Wire Line
	3900 4000 4400 4000
Wire Wire Line
	3900 4100 4400 4100
Wire Wire Line
	3900 4200 4400 4200
Wire Wire Line
	3900 4300 4400 4300
Wire Wire Line
	3900 4400 4400 4400
Wire Wire Line
	3900 4500 4400 4500
Text Label 4400 4500 0    60   ~ 0
gpio_1
Text Label 4400 4400 0    60   ~ 0
gpio_2
Text Label 4400 4300 0    60   ~ 0
gpio_3
Text Label 4400 4200 0    60   ~ 0
gpio_4
Text Label 4400 3600 0    60   ~ 0
GND
Text Label 4400 3700 0    60   ~ 0
3.3V
Text Label 4400 3800 0    60   ~ 0
GND12V
Text Label 4400 3900 0    60   ~ 0
+12VDC
Text Label 6150 3350 2    60   ~ 0
3.3V
Text Label 6150 3850 2    60   ~ 0
3.3V
Text Label 6150 4350 2    60   ~ 0
3.3V
Text Label 6150 4850 2    60   ~ 0
3.3V
Text Label 6150 3150 2    60   ~ 0
gpio_1
Text Label 6150 3650 2    60   ~ 0
gpio_2
Text Label 6150 4150 2    60   ~ 0
gpio_3
Text Label 6150 4650 2    60   ~ 0
gpio_4
Text Label 6750 3650 0    60   ~ 0
GND12V
Text Label 6750 4150 0    60   ~ 0
GND12V
Text Label 6750 4650 0    60   ~ 0
GND12V
$Comp
L R R1
U 1 1 55C0A0EC
P 7050 3350
F 0 "R1" V 7130 3350 50  0000 C CNN
F 1 "1.5k" V 7050 3350 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 6980 3350 30  0000 C CNN
F 3 "" H 7050 3350 30  0000 C CNN
	1    7050 3350
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 55C0A17B
P 7050 4350
F 0 "R3" V 7130 4350 50  0000 C CNN
F 1 "1.5k" V 7050 4350 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 6980 4350 30  0000 C CNN
F 3 "" H 7050 4350 30  0000 C CNN
	1    7050 4350
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 55C0A217
P 7050 4850
F 0 "R4" V 7130 4850 50  0000 C CNN
F 1 "1.5k" V 7050 4850 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 6980 4850 30  0000 C CNN
F 3 "" H 7050 4850 30  0000 C CNN
	1    7050 4850
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 55C0A259
P 7050 3850
F 0 "R2" V 7130 3850 50  0000 C CNN
F 1 "1.5k" V 7050 3850 50  0000 C CNN
F 2 "w_pth_resistors:RC03" V 6980 3850 30  0000 C CNN
F 3 "" H 7050 3850 30  0000 C CNN
	1    7050 3850
	0    1    1    0   
$EndComp
$Comp
L CONN_01X08 P2
U 1 1 55C0A43A
P 8450 3950
F 0 "P2" H 8450 4400 50  0000 C CNN
F 1 "CONN_01X08" V 8550 3950 50  0000 C CNN
F 2 "w_conn_screw:mstba_2,5%2f8-g-5,08" H 8450 3950 60  0000 C CNN
F 3 "" H 8450 3950 60  0000 C CNN
	1    8450 3950
	-1   0    0    -1  
$EndComp
Text Label 8650 3600 0    60   ~ 0
in_1
Text Label 8650 3800 0    60   ~ 0
in_2
Text Label 8650 4000 0    60   ~ 0
in_3
Text Label 8650 4200 0    60   ~ 0
in_4
Text Label 8650 3700 0    60   ~ 0
+12VDC
Text Label 8650 3900 0    60   ~ 0
+12VDC
Text Label 8650 4100 0    60   ~ 0
+12VDC
Text Label 8650 4300 0    60   ~ 0
+12VDC
$EndSCHEMATC

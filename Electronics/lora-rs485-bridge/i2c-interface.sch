EESchema Schematic File Version 5
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
Comment5 ""
Comment6 ""
Comment7 ""
Comment8 ""
Comment9 ""
$EndDescr
Text HLabel 4150 2200 0    50   Input ~ 0
VIN
Text HLabel 5625 2200 2    50   Input ~ 0
VOUT
Text HLabel 5625 2750 2    50   Input ~ 0
SCL
Text HLabel 5625 2875 2    50   Input ~ 0
SDA
Text HLabel 4400 2825 0    50   Input ~ 0
~ENABLE
$Comp
L Device:R R7
U 1 1 5D84EABD
P 5400 2350
F 0 "R7" H 5470 2396 50  0000 L CNN
F 1 "4.7k" H 5470 2305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5330 2350 50  0001 C CNN
F 3 "~" H 5400 2350 50  0001 C CNN
	1    5400 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5D84F22A
P 5125 2350
F 0 "R6" H 5195 2396 50  0000 L CNN
F 1 "4.7k" H 5195 2305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5055 2350 50  0001 C CNN
F 3 "~" H 5125 2350 50  0001 C CNN
	1    5125 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5625 2200 5400 2200
Connection ~ 5400 2200
Wire Wire Line
	5400 2200 5125 2200
Wire Wire Line
	5400 2500 5400 2750
Wire Wire Line
	5400 2750 5625 2750
Wire Wire Line
	5125 2500 5125 2875
Wire Wire Line
	5125 2875 5625 2875
Wire Wire Line
	4900 2200 5125 2200
Connection ~ 5125 2200
$Comp
L Device:R R4
U 1 1 5D8521B9
P 4400 2350
F 0 "R4" H 4331 2304 50  0000 R CNN
F 1 "1M" H 4331 2395 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4330 2350 50  0001 C CNN
F 3 "~" H 4400 2350 50  0001 C CNN
	1    4400 2350
	1    0    0    1   
$EndComp
Wire Wire Line
	4400 2200 4500 2200
$Comp
L Device:R R5
U 1 1 5D853F04
P 4550 2825
F 0 "R5" V 4343 2825 50  0000 C CNN
F 1 "150" V 4434 2825 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4480 2825 50  0001 C CNN
F 3 "~" H 4550 2825 50  0001 C CNN
	1    4550 2825
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 2825 4700 2500
Wire Wire Line
	4400 2500 4400 2825
Wire Wire Line
	4150 2200 4400 2200
Connection ~ 4400 2200
$Comp
L Device:Q_PMOS_GSD Q7
U 1 1 57FC85CD
P 4700 2300
F 0 "Q7" V 4900 2300 50  0000 C CNN
F 1 "Q_PMOS_GSD" V 4937 2300 50  0001 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4900 2400 29  0001 C CNN
F 3 "" H 4700 2300 60  0000 C CNN
F 4 "NX2301P,215 " H 4700 2300 60  0001 C CNN "PartNo"
F 5 "NXP" H 4700 2300 60  0001 C CNN "Manufacturer"
F 6 "1894738" H 4700 2300 60  0001 C CNN "Distributor"
F 7 "Exact as specified" H 4700 2300 50  0001 C CNN "Comment"
	1    4700 2300
	0    1    -1   0   
$EndComp
$EndSCHEMATC

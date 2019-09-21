EESchema Schematic File Version 5
LIBS:lora-bridge-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
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
Text HLabel 2400 1800 0    50   Input ~ 0
VIN
Text HLabel 3725 1800 2    50   Input ~ 0
VOUT
Text HLabel 2400 2425 0    50   Input ~ 0
VBUS
Wire Wire Line
	3500 1800 3725 1800
$Comp
L Device:D_Schottky D?
U 1 1 5D8754F6
P 3350 1800
F 0 "D?" H 3350 1584 50  0000 C CNN
F 1 "D_Schottky" H 3350 1675 50  0000 C CNN
F 2 "" H 3350 1800 50  0001 C CNN
F 3 "~" H 3350 1800 50  0001 C CNN
	1    3350 1800
	-1   0    0    1   
$EndComp
$Comp
L Device:D_TVS D?
U 1 1 5D877A3B
P 2625 1950
F 0 "D?" V 2579 2029 50  0000 L CNN
F 1 "D_TVS" V 2670 2029 50  0000 L CNN
F 2 "" H 2625 1950 50  0001 C CNN
F 3 "~" H 2625 1950 50  0001 C CNN
	1    2625 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 1800 2625 1800
Connection ~ 2625 1800
Wire Wire Line
	2625 1800 3200 1800
$Comp
L Device:D_Schottky D?
U 1 1 5D8754F6
P 3350 2425
F 0 "D?" H 3350 2209 50  0000 C CNN
F 1 "D_Schottky" H 3350 2300 50  0000 C CNN
F 2 "" H 3350 2425 50  0001 C CNN
F 3 "~" H 3350 2425 50  0001 C CNN
	1    3350 2425
	-1   0    0    1   
$EndComp
Wire Wire Line
	3725 1800 3725 2425
Wire Wire Line
	3500 2425 3725 2425
Wire Wire Line
	2400 2425 3200 2425
Text HLabel 2625 2100 0    50   Input ~ 0
GND
$EndSCHEMATC

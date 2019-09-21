EESchema Schematic File Version 5
LIBS:lora-bridge-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 5
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
Text HLabel 4675 2200 2    50   Input ~ 0
A
Text HLabel 4675 2400 2    50   Input ~ 0
B
Text HLabel 4025 1100 0    50   Input ~ 0
VIN
Text HLabel 3175 2900 0    50   Input ~ 0
GND
Text HLabel 3375 2100 0    50   Input ~ 0
DI
Text HLabel 3375 2200 0    50   Input ~ 0
RO
Text HLabel 3175 2400 0    50   Input ~ 0
DE
Text HLabel 2975 2500 0    50   Input ~ 0
~RE
$Comp
L Device:C C7
U 1 1 54DF0C76
P 4025 1400
F 0 "C7" H 4075 1500 50  0000 L CNN
F 1 "1u" H 4075 1300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4063 1250 30  0001 C CNN
F 3 "" H 4025 1400 60  0000 C CNN
	1    4025 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR18
U 1 1 54DF0CCD
P 4025 1550
F 0 "#PWR18" H 4025 1300 60  0001 C CNN
F 1 "GND" H 4025 1400 60  0000 C CNN
F 2 "" H 4025 1550 60  0000 C CNN
F 3 "" H 4025 1550 60  0000 C CNN
	1    4025 1550
	1    0    0    -1  
$EndComp
Text Label 4675 2200 2    60   ~ 0
RS485A
Text Label 4675 2400 2    60   ~ 0
RS485B
Wire Wire Line
	3775 1900 3775 1250
Wire Wire Line
	2975 1250 3775 1250
Wire Wire Line
	4175 2200 4675 2200
Wire Wire Line
	4175 2400 4675 2400
Connection ~ 3775 1250
$Comp
L Device:R R9
U 1 1 58FEF0D0
P 3175 2750
F 0 "R9" V 3255 2750 50  0000 C CNN
F 1 "10k" V 3182 2751 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3105 2750 30  0001 C CNN
F 3 "" H 3175 2750 30  0000 C CNN
	1    3175 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3175 2600 3175 2400
$Comp
L Device:R R3
U 1 1 58FF80B2
P 2975 1850
F 0 "R3" V 3055 1850 50  0000 C CNN
F 1 "10k" V 2982 1851 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2905 1850 30  0001 C CNN
F 3 "" H 2975 1850 30  0000 C CNN
	1    2975 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2975 1250 2975 1700
Wire Wire Line
	2975 2000 2975 2500
Wire Wire Line
	3175 2900 3775 2900
Wire Wire Line
	3775 2900 3775 2700
Wire Wire Line
	3175 2400 3375 2400
Wire Wire Line
	3775 1250 4025 1250
Wire Wire Line
	2975 2500 3375 2500
$Comp
L miceuz:SN65HVD U1
U 1 1 54DF0ADA
P 3775 2300
F 0 "U1" H 3475 2650 50  0000 L CNN
F 1 "SN65HVD72" H 3875 2650 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 3775 2300 50  0001 C CIN
F 3 "" H 3775 2300 60  0000 C CNN
F 4 "SN65HVD72DR" H 3775 2300 60  0001 C CNN "PartNo"
F 5 "TEXAS INSTRUMENTS" H 3775 2300 60  0001 C CNN "Manufacturer"
F 6 "2542747" H 3775 2300 60  0001 C CNN "Distributor"
	1    3775 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4025 1100 4025 1250
Connection ~ 4025 1250
$EndSCHEMATC

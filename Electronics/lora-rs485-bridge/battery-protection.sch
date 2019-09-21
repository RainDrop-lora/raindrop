EESchema Schematic File Version 5
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
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
L Device:D_Schottky D2
U 1 1 5D8754F6
P 3350 1800
F 0 "D2" H 3350 1584 50  0000 C CNN
F 1 "D_Schottky" H 3350 1675 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123F" H 3350 1800 50  0001 C CNN
F 3 "~" H 3350 1800 50  0001 C CNN
F 4 "ON SEMICONDUCTOR" H 3350 1800 50  0001 C CNN "Manufacturer"
F 5 "MBR1020VL" H 3350 1800 50  0001 C CNN "PartNo"
	1    3350 1800
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D3
U 1 1 5D86D14C
P 3350 2425
F 0 "D3" H 3350 2209 50  0000 C CNN
F 1 "D_Schottky" H 3350 2300 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-123F" H 3350 2425 50  0001 C CNN
F 3 "~" H 3350 2425 50  0001 C CNN
F 4 "ON SEMICONDUCTOR" H 3350 2425 50  0001 C CNN "Manufacturer"
F 5 "MBR1020VL" H 3350 2425 50  0001 C CNN "PartNo"
	1    3350 2425
	-1   0    0    1   
$EndComp
Wire Wire Line
	3725 1800 3725 2425
Wire Wire Line
	3500 2425 3725 2425
Wire Wire Line
	2400 2425 3200 2425
Text HLabel 2775 2150 0    50   Input ~ 0
GND
$Comp
L Device:D_TVS_x2_AAC D5
U 1 1 5D880D1E
P 2775 2000
F 0 "D5" H 2775 2125 50  0000 C CNN
F 1 "D_TVS_x2_AAC" H 2775 2125 50  0001 C CNN
F 2 "miceuz-lib:SC89" H 2625 2000 50  0001 C CNN
F 3 "~" H 2625 2000 50  0001 C CNN
F 4 "ON SEMICONDUCTOR" H 725 300 50  0001 C CNN "Manufacturer"
F 5 " NZL5V6AXV3T1G" H 725 300 50  0001 C CNN "PartNo"
	1    2775 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 1800 2425 1800
Wire Wire Line
	2425 2000 2425 1800
Connection ~ 2425 1800
Wire Wire Line
	2425 1800 3125 1800
Wire Wire Line
	3125 2000 3125 1800
Connection ~ 3125 1800
Wire Wire Line
	3125 1800 3200 1800
$EndSCHEMATC

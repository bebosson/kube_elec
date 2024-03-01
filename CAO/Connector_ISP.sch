EESchema Schematic File Version 4
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
$EndDescr
$Comp
L Kube_library:Conn_01x06_Male J1
U 1 1 60996435
P 4400 3450
F 0 "J1" H 4383 3875 50  0000 C CNN
F 1 "Conn_01x06_Male" H 4383 3784 50  0000 C CNN
F 2 "Le_kube:AVR-ISP(IDC-TH_6P-P2.54_C11214)" H 4400 3450 50  0001 C CNN
F 3 "~" H 4400 3450 50  0001 C CNN
	1    4400 3450
	1    0    0    -1  
$EndComp
Text HLabel 4600 3250 2    50   Input ~ 0
MISO
Text HLabel 4600 3450 2    50   Input ~ 0
SCK
Text HLabel 4600 3550 2    50   Input ~ 0
MOSI
Text HLabel 4600 3650 2    50   Input ~ 0
RST
$Comp
L power:GND #PWR0121
U 1 1 609993AE
P 4600 3750
F 0 "#PWR0121" H 4600 3500 50  0001 C CNN
F 1 "GND" V 4605 3622 50  0000 R CNN
F 2 "" H 4600 3750 50  0001 C CNN
F 3 "" H 4600 3750 50  0001 C CNN
	1    4600 3750
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR0122
U 1 1 60997C3A
P 4600 3350
F 0 "#PWR0122" H 4600 3200 50  0001 C CNN
F 1 "+3.3V" V 4615 3478 50  0000 L CNN
F 2 "" H 4600 3350 50  0001 C CNN
F 3 "" H 4600 3350 50  0001 C CNN
	1    4600 3350
	0    1    1    0   
$EndComp
$EndSCHEMATC

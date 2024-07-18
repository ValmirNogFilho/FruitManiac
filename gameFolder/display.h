#ifndef DISPLAY_H
#define DISPLAY_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

// Constants for 7-segment display codes
#define zero 0b1000000
#define one 0b1111001
#define two 0b0100100
#define three 0b0110000
#define four 0b0011001
#define five 0b0010010
#define six 0b0000010
#define seven 0b1111000
#define eight 0b0000000
#define nine 0b0010000

// Function prototypes
extern void setToLifeDigit(int n);
extern void setToHundredsDigit(int n);
extern void setToDozensDigit(int n);
extern void setToUnitsDigit(int n);

#endif // DISPLAY_H

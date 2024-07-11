#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "enderecos.h"

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

volatile int * DISPLAY_ptr;
volatile int * DISPLAY_ptr1;
volatile int * DISPLAY_ptr2;
volatile int * DISPLAY_ptr3;
volatile int * DISPLAY_ptr4;
volatile int * DISPLAY_ptr5;
volatile int * key;
int fd = -1;
void *LW_virtual;

int open_physical_file (int fd) {
    if (fd == -1)
        if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf ("ERROR: could not open \"/dev/mem\"...\n");
            return (-1);
        }
    return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical_file (int fd) {
    close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span) {
    void *virtual_base;

    // Get a mapping from physical addresses to virtual addresses
    virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
    if (virtual_base == MAP_FAILED) {
        printf ("ERROR: mmap() failed...\n");
        close (fd);
        return (NULL);
    }
    return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void * virtual_base, unsigned int span) {
    if (munmap (virtual_base, span) != 0) {
        printf ("ERROR: munmap() failed...\n");
        return (-1);
    }
    return 0;
}


int linkNumberTo7SegCode(int numero) {
    switch (numero) {
        case 9:
            return nine;
        case 8:
            return eight;
        case 7:
            return seven;
        case 6:
            return six;
        case 5:
            return five;
        case 4:
            return four;
        case 3:
            return three;
        case 2:
            return two;
        case 1:
            return one;
        case 0:
            return zero;
        default:
            printf("Número %d não tem correspondência definida.\n", numero);
            break;
    }
}

void displayMapping() {
    // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical_file (fd)) == -1)
        return (-1);
    if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
        return (-1);

    // Set virtual address pointer to I/O port
    DISPLAY_ptr = (unsigned int *) (LW_virtual + HEX0_BASE);

    DISPLAY_ptr1 = (unsigned int *) (LW_virtual + HEX1_BASE);

    DISPLAY_ptr2 = (unsigned int *) (LW_virtual + HEX2_BASE);

    DISPLAY_ptr3 = (unsigned int *) (LW_virtual + HEX3_BASE);

    DISPLAY_ptr4 = (unsigned int *) (LW_virtual + HEX4_BASE);

    DISPLAY_ptr5 = (unsigned int *) (LW_virtual + HEX5_BASE);

    key = (unsigned int *) (LW_virtual + KEYS_BASE);

}


void setToLifeDigit(int n) {
    *DISPLAY_ptr4=linkNumberTo7SegCode(n); 	
}

void setToHundredsDigit(int n){
    *DISPLAY_ptr=linkNumberTo7SegCode(n); 

}

void setToDozensDigit(int n){
    *DISPLAY_ptr1=linkNumberTo7SegCode(n);    

}

void setToUnitsDigit(int n){
    *DISPLAY_ptr2=linkNumberTo7SegCode(n);	
}
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "enderecos.h"
#include "address_map_arm.h"

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);


#define nove 0b0010000


#define oito 0b0000000


#define sete 0b1111000


#define seis 0b0000010


#define cinco 0b0010010


#define quatro 0b0011001

#define tres 0b0110000

#define dois 0b0100100

#define zero 0b1000000

#define um 0b1111001








#define espaco 0b1111111


#define botao 0x0000005C

/* This program increments the contents of the red LED parallel port */
int main(void)
{
    int letras[17];// ={P,B,L,traco,S,D,espaco,t,P,zero,um};
    letras[0] = cinco;
    letras[1] = quatro;
    letras[2] = tres;
    letras[3] = dois;
    letras[4] = um;
    letras[5] = zero;
    letras[6] = espaco;
    letras[7] = espaco;
    letras[8] = espaco;
    letras[9] = seis;
    letras[10] = sete;
    letras[11] = oito;
    letras[12] = nove;
    letras[13] = espaco;
    letras[14] = espaco;
    letras[15] = espaco;
    letras[16] = espaco;
    
 
		
  
    volatile int * DISPLAY_ptr;
    volatile int * DISPLAY_ptrum;
    volatile int * DISPLAY_ptrdois;
    volatile int * DISPLAY_ptrtres;
    volatile int * DISPLAY_ptrquarto;
    volatile int * DISPLAY_ptrquinto;
    volatile int * chave;

    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   DISPLAY_ptr = (unsigned int *) (LW_virtual + HEX0_BASE);
   
   DISPLAY_ptrum = (unsigned int *) (LW_virtual + HEX1_BASE);

   DISPLAY_ptrdois = (unsigned int *) (LW_virtual + HEX2_BASE);
   
   DISPLAY_ptrtres = (unsigned int *) (LW_virtual + HEX3_BASE);

   DISPLAY_ptrquarto = (unsigned int *) (LW_virtual + HEX4_BASE);
   
   DISPLAY_ptrquinto = (unsigned int *) (LW_virtual + HEX5_BASE);
    
   chave = (unsigned int *) (LW_virtual + KEYS_BASE);
   *DISPLAY_ptr = 0; 
   *DISPLAY_ptrum = 0; 
    int botao_pressionado=0;
    int pause=0;
    *chave=15;

    int i = 0;

    while(1){

        if(*chave==1){
                botao_pressionado++;
                *chave=15;
            }

        if(botao_pressionado==2){
                pause=0;
                botao_pressionado=0;
        }
        

        for(i; i < 12 && pause==0; i++) {
            
            if(*chave==1){
                botao_pressionado++;
                *chave=15;
            }

            if(botao_pressionado==1)
                 pause=1;
            
            
                


            printf("%d",pause);
            * DISPLAY_ptrquinto= letras[i];
            * DISPLAY_ptrquarto = letras[i+1];
            * DISPLAY_ptrtres= letras[i+2];
            * DISPLAY_ptrdois = letras[i+3];
            * DISPLAY_ptrum= letras[i+4];
            * DISPLAY_ptr = letras[i+5];
            sleep(1);
        }

        if(i>=12) i = 0;

    
    }
}



// Open /dev/mem, if not already done, to give access to physical addresses
int open_physical (int fd) {
    if (fd == -1)
        if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf ("ERROR: could not open \"/dev/mem\"...\n");
            return (-1);
        }
    return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical (int fd) {
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

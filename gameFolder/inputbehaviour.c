#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>
#include "inputbehaviour.h"
#include "../lib/gpulib.h"
#include "addresses.h"

#define B1 14
#define B2 13 
#define B3 11
#define B4 7

struct input_event ev;
pthread_mutex_t lock;
int fd_mouse;
int xsoma = 0, ysoma = 0;
unsigned int click_reset = 0;

sprite_t beam = {4, BEAM_VERTICAL, 0, 0, 0};


int start = 1;
int end;
int restart;
int stop;


void* read_mouse(void* arg) {
    
    ssize_t n;
    int x, y;

    while (1) {
        n  = read(fd_mouse, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            perror("Error reading");
            continue;
        } else if (n != sizeof(ev)) {
            fprintf(stderr, "Error: read %ld bytes, expecting %ld\n", n, sizeof(ev));
            continue;
        }

        pthread_mutex_lock(&lock);

        if (ev.type == EV_REL && ev.code == REL_X) {
            x = ev.value;
            xsoma += x;
        }
        if (ev.type == EV_REL && ev.code == REL_Y) {
            y = ev.value;
            ysoma += y;
        }

        // Limitar as coordenadas acumuladas para evitar overflow
        if (xsoma < 0) xsoma = 0;
        if (xsoma > 619) xsoma = 619;
        if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1 && beam.rel_y == 0) {
            click_reset = ev.value;
            
        
        }

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

// Função da thread para leitura do mouse
void* read_key(void* arg) {
    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
   // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physicall (fd)) == -1)
        return (-1);
    if ((LW_virtual = map_physicall (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
        return (-1);


        
    volatile int * key;
    key = (unsigned int *) (LW_virtual + KEYS_BASE);
    ssize_t n;
    int x, y;
    int pause_state;
    int restart_state=1;
    int start_state=1;
    int end_state=1;
    pause_state=1;
    restart=0;
    stop =1;
    end=0;


    while (1) {

        if(*key==B1 && pause_state ==1){
            pause_state=0; 
            if(stop == 1) {
               stop = 0;
            } else {
              stop = 1;
            }

        }

        if(*key==B2 && restart_state ==1){
            restart_state=0; 
            if (restart == 0) {
            restart = 1;
            } 
        }

        if(*key==B3 && end_state ==1){
            end_state=0; 
            if (end == 0) {
                end = 1;
            } 
        }

        if(*key==B4 && start_state ==1){
            start_state=0; 
            if (start == 0) {
                start = 1;
            } 
        }
        if(*key==15 && start_state==0){
            start_state=1;
            start=0;
        }

        if(*key==15 && restart_state==0){
            restart_state=1;
            restart=0;


        }

        if(*key==15 && pause_state==0){
            pause_state=1;
        }

    }
    return NULL;
}

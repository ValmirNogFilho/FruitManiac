
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <string.h>
#include "lib/gpulib.h"
#include <time.h>

#define DEVICE_PATH "/dev/gpu123"
#define MOUSE_DEVICE "/dev/input/event0"
#define MAX 511

int main() {
    int xsoma = 0, ysoma = 0;
    int x, y;
    int fd_mouse, fd_gpu;
    struct input_event ev;
    ssize_t n;
    sprite_t spr;
    sprite_t beam;
    color_t color = {0,3,0};
    color_t click = {0, 7, 0};
    unsigned int block;
    int i;
    color_t reset = {6, 7, 7};

    // polygon_t p;
    // p.address = 0;
    // p.color.R = 7;
    // p.color.G = 0;
    // p.color.B = 0;
    // p.shape = SQUARE;
    // p.size = s100x100;
    // p.rel_x = 50;
    // p.rel_y = 50;
    
    beam.address = 3;
    beam.rel_y = 439;
    beam.variation = BEAM_VERTICAL;
    beam.visible = 0;


    // for (i = 0; i < 4800; i++)
    //     editBlockOnBackgroundMemory(i, reset);

    setBackground(color);

    // Abrir o dispositivo do mouse
    fd_mouse = open(MOUSE_DEVICE, O_RDONLY);
    if (fd_mouse == -1) {
        perror("Cannot open input device");
        return 1;
    }

    // Abrir o dispositivo da GPU
    fd_gpu = open(DEVICE_PATH, O_WRONLY);
    if (fd_gpu == -1) {
        perror("Cannot open GPU device");
        close(fd_mouse);
        return 1;
    }
    // Loop infinito para receber as coordenadas do mouse
    while (1) {
        // Ler o evento do mouse
        n = read(fd_mouse, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            perror("Error reading");
            break;
        } else if (n != sizeof(ev)) {
            fprintf(stderr, "Error: read %ld bytes, expecting %ld\n", n, sizeof(ev));
            break;
        }

        if (ev.type == EV_REL && ev.code == REL_X) {
            x = ev.value;
        }
        if (ev.type == EV_REL && ev.code == REL_Y) {
            y = ev.value;
        }
        if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1) {
            beam.rel_x = xsoma;
            beam.visible = 1;
            beam.rel_y = 439;
        }

        // Atualiza as coordenadas acumuladas
        xsoma += x;
        ysoma += y;
        


        // Limitar as coordenadas acumuladas para evitar overflow
        if (xsoma < 0) xsoma = 0;
        if (xsoma > 619) xsoma = 619;

        if(beam.rel_y >= 11 && beam.visible == 1) {
            beam.rel_y -= 1;
        }
        setSpriteOnScreen(beam);

        
        // Criar a estrutura do sprite com as novas coordenadas
        spr.address = 2;
        spr.rel_x = xsoma;
        spr.rel_y = 459;
        spr.variation = ENEMY_1;
        spr.visible = 1;

        setSpriteOnScreen(spr);
        
    }

    // Fechar os dispositivos
    close(fd_mouse);
    close(fd_gpu);
    return 0;
}

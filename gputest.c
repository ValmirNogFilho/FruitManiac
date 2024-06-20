#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <string.h>
#include "lib/gpulib.h"

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
    color_t color = {0,3,0};
    color_t click = {0, 7, 0};
    unsigned int block;
    int i;
    color_t reset = {6, 7, 7};
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
        if (ev.type == EV_KEY && ev.code == BTN_LEFT) {
            // xsoma*80 + ysoma
            // ysoma*80 + xsoma
            // xsoma*60 + ysoma
            block = (xsoma / 8) + (ysoma/8)*80;
            if(ev.value == 1) {
                editBlockOnBackgroundMemory(block, click);
            }
        }

        // Atualiza as coordenadas acumuladas
        xsoma += x;
        ysoma += y;
        
        // Limitar as coordenadas acumuladas para evitar overflow
        if (xsoma < 0) xsoma = 0;
        if (xsoma > 619) xsoma = 619;
        if (ysoma < 0) ysoma = 0;
        if (ysoma > 459) ysoma = 459;
        
        // Criar a estrutura do polígono com as novas coordenadas
        spr.address = 1;
        spr.rel_x = xsoma;
        spr.rel_y = ysoma;
        spr.variation = TREE;
        spr.visible = 1;

        // Serializar os dados do polígono em um buffer
        setSpriteOnScreen(spr);

    }

    // Fechar os dispositivos
    close(fd_mouse);
    close(fd_gpu);
    return 0;
}

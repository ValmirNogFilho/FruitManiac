#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "lib/gpulib.h"
#include <time.h>

#define DEVICE_PATH "/dev/gpu123"
#define MOUSE_DEVICE "/dev/input/event0"
#define MAX 511

int xsoma = 0, ysoma = 0;
int fd_mouse;
pthread_mutex_t lock;
struct input_event ev;

unsigned int rel_x;
unsigned int address = 3;
unsigned int rel_y = 439;
sprite_variation_t variation = BEAM_VERTICAL;
unsigned int visible = 0;
unsigned int click2 = 0;

// Função da thread para leitura do mouse
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
        if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1) {
            click2 = ev.value;
            
        }

        

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    int x, y;
    int fd_gpu;
    sprite_t spr;
    sprite_t beam;
    color_t color = {0, 3, 0};
    color_t click = {0, 7, 0};
    unsigned int block;
    int i;
    color_t reset = {6, 7, 7};
    pthread_t mouse_thread;

    beam.address = 3;
    beam.rel_y = 439;
    beam.variation = BEAM_VERTICAL;
    beam.visible = 0;

    setBackground(color);

    // Abrir o dispositivo do mouse
    fd_mouse = open(MOUSE_DEVICE, O_RDONLY );
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

    // Inicializar o mutex
    pthread_mutex_init(&lock, NULL);

    // Criar a thread para leitura do mouse
    pthread_create(&mouse_thread, NULL, read_mouse, NULL);

    // Loop principal
    while (1) {
        // Atualiza as coordenadas acumuladas
        pthread_mutex_lock(&lock);
        x = xsoma;
        y = ysoma;
        if(click2 == 1){
            beam.visible = 1;
            beam.rel_y = 439;
            beam.rel_x = xsoma;
            click2=0;
        }
        
        if (beam.rel_y >= 11 && beam.visible == 1) {
            beam.rel_y -= 1;
        }
        
        pthread_mutex_unlock(&lock);

        
        printf("Auwq\n");
        setSpriteOnScreen(beam);



        // Criar a estrutura do sprite com as novas coordenadas
        spr.address = 2;
        spr.rel_x = x;
        spr.rel_y = 459;
        spr.variation = ENEMY_1;
        spr.visible = 1;

        setSpriteOnScreen(spr);
        printf("Antes do click\n");
        // Verifica se o botão do mouse foi pressionado
        
    }

    // Fechar os dispositivos
    close(fd_mouse);
    close(fd_gpu);

    // Destruir o mutex
    pthread_mutex_destroy(&lock);

    return 0;
}

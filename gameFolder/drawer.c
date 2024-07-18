#include "../lib/gpulib.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "drawer.h"


void draw(unsigned char* path, int initial_address){
    int x, y, endereco;
    color_t cor = {6, 7, 7};
    color_t blue = {0,0,7};
    FILE *file;
    int r, g, b;
    int i, j;
    int address;
    color_t color;


    // Abra o arquivo em modo leitura
    file = fopen(path, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Leia cada linha e separe os valores
    for(i = 0; i < 20; i++) {
        for(j = 0; j < 20; j++) {
            address = initial_address + 20 * j + i;
            if(fscanf(file, "%d %d %d", &r, &g, &b) != EOF) {
                color.R = r;
                color.G = g;
                color.B = b;

                setPixelOnSpriteMemory(address, color);
            }
        }
    }

    fclose(file);

    return;

}

void draw_apple_on_memory() {
    draw("gameFolder/sprites/apple.sprite", 10000); //endereço 25
}

void draw_orange_on_memory() {
    draw("gameFolder/sprites/orange.sprite", 10400); //endereço 26
}

void draw_pear_on_memory() {
    color_t transparent = {6, 7, 7};
    int j;

    draw("gameFolder/sprites/pear.sprite", 10800); //endereço 27
    for(j = 0; j < 20; j++) {
            setPixelOnSpriteMemory(11200+j, transparent); //eliminando linha extra exibida pela gpu
    }
}



void erase_bg_screen() {
    int block_address;
    color_t apagar = {6, 7, 7};
    for(block_address = 0; block_address < 4800; block_address++) {
        editBlockOnBackgroundMemory(block_address, apagar);
    }
}

void set_screen(char * path) {
    FILE *file;
    int r, g, b;
    int i, j;
    int address;
    color_t color;
    // Abra o arquivo em modo leitura
    file = fopen(path, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for(j = 0; j < 80; j++) {
        for(i = 0; i < 60; i++) {
            address = 80 * i + j;
            if(fscanf(file, "%d %d %d", &r, &g, &b) != EOF) {
                color.R = r;
                color.G = g;
                color.B = b;
                editBlockOnBackgroundMemory(address, color);
            }
        }
    }
    fclose(file);
}

void set_start_screen() {
    set_screen("gameFolder/sprites/inicio.back");
}

void set_pause_screen() {
    set_screen("gameFolder/sprites/pause.back");
}

void set_game_over_screen() {
    set_screen("gameFolder/sprites/fim.back");
}

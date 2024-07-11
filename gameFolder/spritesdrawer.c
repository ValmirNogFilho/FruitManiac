#include "../lib/gpulib.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

color_t green = {0,7,0};
color_t red = {7, 0, 0};
color_t white = {7,7,7};
color_t brown = {6,3,0};
color_t bg = {0, 3, 0};
color_t transparent = {6, 7, 7};

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
    draw("sprites/apple.sprite", 10000); //endereço 25
}

void draw_orange_on_memory() {
    draw("sprites/orange.sprite", 10400); //endereço 26
}

void draw_pear_on_memory() {
    draw("sprites/pear.sprite", 10800); //endereço 27
    int j;
    for(j = 0; j < 20; j++) {
            setPixelOnSpriteMemory(11200+j, transparent); //eliminando linha extra exibida pela gpu
    }
}

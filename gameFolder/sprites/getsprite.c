#include <stdio.h>
#include "lib/gpulib.h"

int main() {
    FILE *file;
    int r, g, b;
    int i, j;
    int address;
    color_t color;
    // Abra o arquivo em modo leitura
    file = fopen("apple.sprite", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Leia cada linha e separe os valores
    for(i = 0; i < 20; i++) {
        for(j = 0; j < 20; j++) {
            address = 10000 + 20 * i + j;
            if(fscanf(file, "%d %d %d", &r, &g, &b) != EOF) {
                color.R = r;
                color.G = g;
                color.B = b;

                setPixelOnSpriteMemory(address, color);
            }
        }
    }

    // while (fscanf(file, "%d %d %d", &val1, &val2, &val3) != EOF) {
    //     printf("Valores lidos: %d, %d, %d\n", val1, val2, val3);
    // }

    // Feche o arquivo
    fclose(file);

    return 0;
}

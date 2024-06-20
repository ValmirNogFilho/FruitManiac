#include "instructions.h"
#include "datastructures.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define BYTE_BITS 8
#define DEVICE "/dev/gpu123"

int open_physical (int);
void close_physical (int);

char lastRegister = 0;

void
printBits(char* word) {
    int i;
    int j;
    for (i = 7; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            printf("%d", (word[i] >> j) & 1);
        }
        printf(" "); // Espaço a cada byte
    }
    printf("\n");
}

int
writeBitsOnDeviceDriver(unsigned char* bits, char* error_msg){
    int fd;
    if ((fd = open_physical (fd)) == -1)
        return (-1);

    ssize_t buffer =  write(fd,bits,8); //escreve buffer de 8 bytes no kernel, com os dados para DATA_A e DATA_B

    unsigned char readData[8];
    ssize_t bytesRead=read(fd,readData,8); //recebe retorno do kernel para verificar se a escrita foi correta
    if (bytesRead<0) {
        printf("%s", error_msg);
        close_physical (fd);
        return bytesRead;
    }

    close_physical (fd); // fecha char device driver
    return 0;   
}

//as funções a seguir formam a palavra da instrução e chamam a função de envio para o kernel.
//Ao final, retornam se a operação foi concluída com sucesso.

int
setBackground(color_t color) {
    char* word = assembleInstructionWBR(color.R, color.G, color.B);
    return writeBitsOnDeviceDriver(word, "erro na escrita do background");
   
}

int
setPixelOnSpriteMemory(unsigned int mem_address, color_t color) {
    char* word = assembleInstructionWSM(mem_address, color.R, color.G, color.B);
    return writeBitsOnDeviceDriver(word, "erro na edição de pixel na memória de sprites");
}

int
editBlockOnBackgroundMemory(unsigned int block, color_t color) {
    unsigned char* word = assembleInstructionWBM(block, color.R, color.G, color.B);
    return writeBitsOnDeviceDriver(word, "erro na edição de bloco na memória de background");
}

int
setSpriteOnScreen(sprite_t sprite) {
    unsigned char* word = assembleInstructionWBR_2(sprite.address, sprite.variation, sprite.rel_y, sprite.rel_x, sprite.visible);
    return writeBitsOnDeviceDriver(word, "erro na escrita de sprite");
}

int
setPolygon(polygon_t polygon) {
    unsigned char* word = assembleInstructionDP(polygon.rel_x, polygon.rel_y, polygon.address,
    polygon.size, polygon.color.R, polygon.color.G, polygon.color.B, polygon.shape);
    return writeBitsOnDeviceDriver(word, "erro na escrita de polígono");
}

int
eraseBackground() {
    color_t color = {7,7,7};
    return setBackground(color); //apenas pinta a tela de preto
}


int
open_physical (int fd) {
    if ((fd = open( "/dev/gpu123", (O_RDWR | O_SYNC))) == -1) { //verifica existência e legibilidade do char device driver
        printf ("ERROR: could not open \"/dev/mem\"...\n");
        return (-1);
    }
    return fd;
}




void
close_physical (int fd) {
    close (fd);
}


#include "datastructures.h"

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

//gera binário para a instrução de escrita de cor em background
unsigned char* assembleInstructionWBR(
    unsigned char R, unsigned char G, unsigned char B);

//gera binário para a instrução de escrita de sprite na tela
unsigned char* assembleInstructionWBR_2(
    unsigned char reg, unsigned int offset,
    unsigned int Coord_y, unsigned int Coord_x, unsigned char sp);

//gera binário para a instrução de escrita de novo pixel ou edição de pixel anterior na memória de sprites
unsigned char* assembleInstructionWSM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B);

//gera binário para a instrução de escrita de cor em um bloco específico do background
unsigned char* assembleInstructionWBM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B);

//gera binário para a instrução de escrita de polígono na tela
unsigned char* assembleInstructionDP(
    unsigned int ref_point_X, unsigned int ref_point_Y, unsigned char address,
    unsigned char size, unsigned char R, unsigned char G, unsigned char B, unsigned char shape);

#endif

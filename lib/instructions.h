#include "datastructures.h"

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

//gera binário para a instrução de escrita de cor em background
// unsigned char* assembleInstructionWBR(
void assembleInstructionWBR(
    unsigned char R, unsigned char G, unsigned char B, unsigned char word[]);

//gera binário para a instrução de escrita de sprite na tela
void assembleInstructionWBR_2(
    unsigned char reg, unsigned int offset,
    unsigned int Coord_y, unsigned int Coord_x, unsigned char sp, unsigned char word[]);

//gera binário para a instrução de escrita de novo pixel ou edição de pixel anterior na memória de sprites
// unsigned char* 
void assembleInstructionWSM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B, unsigned char word[]);

//gera binário para a instrução de escrita de cor em um bloco específico do background
void assembleInstructionWBM(
    unsigned int address, unsigned char R, unsigned char G, unsigned char B, unsigned char word[]);

//gera binário para a instrução de escrita de polígono na tela
void assembleInstructionDP(
    unsigned int ref_point_X, unsigned int ref_point_Y, unsigned char address,
    unsigned char size, unsigned char R, unsigned char G, unsigned char B, unsigned char shape, unsigned char word[]);

#endif

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

typedef enum { // para definição do formato dos polígonos 
  SQUARE   = 0,
  TRIANGLE = 1
} Shape;

typedef enum size { // tamanhos disponíveis para os polígonos da gpu
  off      = 0b0000,
  s20x20   = 0b0001, 
  s30x30   = 0b0010,
  s40x40   = 0b0011, 
  s50x50   = 0b0100,
  s60x60   = 0b0101,
  s70x70   = 0b0110,
  s80x80   = 0b0111,
  s90x90   = 0b1000,
  s100x100 = 0b1001,
  s110x110 = 0b1010,
  s120x120 = 0b1011,
  s130x130 = 0b1100, 
  s140x140 = 0b1101,
  s150x150 = 0b1110,
  s160x160 = 0b1111
} polygon_size_t;

typedef enum {
  ENEMY_1,
  ENEMY_2,
  MEDAL,
  BOMB,
  TREE,
  PLANE_UP,
  PLANE_RIGHT,
  PLANE_DOWN,
  PLANE_LEFT,
  COIN,
  DIAMOND,
  BEAM_VERTICAL,
  BEAM_DIAG_RIGHT,
  BEAM_HORIZONTAL,
  BEAM_DIAG_LEFT,
  HEART,
  LOG,
  F1_CAR_1,
  F1_CAR_2,
  F1_CAR_3,
  F1_CAR_4,
  ALIEN_1,
  ALIEN_2,
  ROCK,
  TROPHY
} sprite_variation_t;

typedef struct {
  unsigned char R;
  unsigned char G;
  unsigned char B;
} color_t;

typedef struct {
  unsigned char address;
  polygon_size_t size;
  color_t color;
  Shape shape;
  unsigned int rel_x;
  unsigned int rel_y;
} polygon_t;

typedef struct {
  unsigned int address;
  sprite_variation_t variation;
  unsigned int rel_x;
  unsigned int rel_y;
  unsigned char visible;
} sprite_t;

#endif

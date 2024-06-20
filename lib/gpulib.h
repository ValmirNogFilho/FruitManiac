#include "datastructures.h"

int setBackground(color_t color);

int setPixelOnSpriteMemory(unsigned int mem_address, color_t color);

int editBlockOnBackgroundMemory(unsigned int block, color_t color);

int setSpriteOnScreen(sprite_t sprite);

int setPolygon(polygon_t polygon);

int eraseBackground();
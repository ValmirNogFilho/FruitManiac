#ifndef GAME_GRAPHICS_H
#define GAME_GRAPHICS_H

#include "../lib/gpulib.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

void draw_apple_on_memory();
void draw_orange_on_memory();
void draw_pear_on_memory();
void erase_bg_screen();
void set_start_screen();
void set_pause_screen();
void set_game_over_screen();
void set_bg_screen();

#endif // GAME_GRAPHICS_H

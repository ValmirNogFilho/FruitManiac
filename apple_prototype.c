// apple.piskel: {"modelVersion":2,"piskel":{"name":"New Piskel","description":"","fps":12,"height":20,"width":20,"layers":["{\"name\":\"Layer 1\",\"opacity\":1,\"frameCount\":1,\"chunks\":[{\"layout\":[[0]],\"base64PNG\":\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAAAXNSR0IArs4c6QAAAJpJREFUOE9jZKAyYCTFPO408f8w9V9nvcSql/4Geumww1110EoA7iGyXYhs4LYrPwn6iKACqhj4WZUB7k18kcZ7mwHDQRgCxBoGswjdUBQDSTUMm6FwA8kxjPvmP4av6kxgc2EuHTWQlJzNwEDVMAQZBgJUixRk12GNZZAgMUkH3WXIhoHYtM0psCghxqXoLoPpJVjakBbvDAwAhIRUFY7+EfAAAAAASUVORK5CYII=\"}]}"],"hiddenFrames":[""]}}

#include "lib/gpulib.h"

color_t green = {0,7,0};
color_t red = {7,0,0};
color_t white = {7,7,7};
color_t brown = {6,3,0};
color_t bg = {6, 7, 7};

int i, j;
//setPixelOnSpriteMemory(mem_address, color);


int mem_address(int initial_mem_addr, int column, int row){
    return initial_mem_addr + row * 20 + column;
}

void draw_apple() {
  for(i = 0; i < 20; i++)
      for(j = 0; j < 20; j++)
          setPixelOnSpriteMemory(mem_address(10000, i, j), bg);
  
  for(i = 11; i <= 13; i++)
      for(j = 1; j <= 3; j++)
          setPixelOnSpriteMemory(mem_address(10000, i, j), green);
  
  for(i = 8; i <= 10; i++)
      for(j = 3; j <= 5; j++)
          setPixelOnSpriteMemory(mem_address(10000, i, j), green);
  
  for (i = 6; i <= 18; i++) {
          for (j = 5; j <= 14; j++) {
              setPixelOnSpriteMemory(mem_address(10000, i, j), red);
          }
      }
  
  for (i = 7; i <= 17; i++){
      setPixelOnSpriteMemory(mem_address(10000, i, 4), red);
  }
  
  for (i = 8; i <= 16; i++){
      setPixelOnSpriteMemory(mem_address(10000, i, 3), red);
  }
  
  for (i = 9; i <= 15; i++){
      setPixelOnSpriteMemory(mem_address(10000, i, 2), red);
  }
  
  
  for (i = 7; i <= 17; i++){
      setPixelOnSpriteMemory(mem_address(10000, i, 15), red);
  }
  
  
  for (i = 8; i <= 16; i++){
      setPixelOnSpriteMemory(mem_address(10000, i, 16), red);
  }
  
  for (i = 9; i <= 15; i++){
      setPixelOnSpriteMemory(mem_address(10000, i, 17), red);
  }
  
  for (i = 9; i <= 14; i++){
      setPixelOnSpriteMemory(mem_address(10000, i, 15), white);
  }
  
  setPixelOnSpriteMemory(mem_address(10000, 14, 14), white);
  setPixelOnSpriteMemory(mem_address(10000, 14, 15), white);
  setPixelOnSpriteMemory(mem_address(10000, 13, 16), white);
  setPixelOnSpriteMemory(mem_address(10000, 12, 16), white);  
}


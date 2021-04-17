#ifndef SDL2_UTIL_H
#define SDL2_UTIL_H

#include <cstdint>
#include <SDL2/SDL.h> /* Video feature */

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

struct PixelInfo
{
  PixelInfo(uint32_t x, uint32_t y, uint32_t r, uint32_t g, uint32_t b){
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;
  }
  int x;
  int y;

  uint32_t r;
  uint32_t g;
  uint32_t b;
};

int monitor_sdl_open(void);
int monitor_sdl_close(void);
int sdl_event_poll();
void Draw_Triangle();

#endif /*SDL2_UTIL_H*/
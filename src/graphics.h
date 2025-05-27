#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include <SDL2/SDL.h>
#include "config.h"

typedef struct _graphics {
  SDL_Window *window;
  SDL_Renderer *renderer;
} graphics_t;

void graphics_init (graphics_t *gfx, config_t *config);
void graphics_close (graphics_t *gfx);

#endif

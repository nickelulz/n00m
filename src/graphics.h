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

void graphics_draw_text (graphics_t *gfx, TTF_Font *font, const char *text,
			 SDL_Color text_color, SDL_Color background_color,
			 int x, int y, int padding);

#endif

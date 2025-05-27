#ifndef STATE_HEADER
#define STATE_HEADER

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "config.h"

typedef struct _state {
  SDL_Color background_color, rectangle_color;
  SDL_Rect rectangle;
  bool running;
} state_t;

void state_init (state_t *state, config_t *config);
void state_close (state_t *state);

#endif

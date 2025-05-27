#ifndef STATE_HEADER
#define STATE_HEADER

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "config.h"
#include "player.h"

typedef struct _state {
  player_t player;
  bool running;
} state_t;

void state_init (state_t *state, config_t *config);
void state_close (state_t *state);

#endif

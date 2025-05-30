#include <stdint.h>
#include <string.h>
#include "lib_common.h"

#include "color.h"
#include "state.h"
#include "config.h"
#include "util.h"

int
map_at_vec (state_t *state, ivec2s pos)
{
  if (!in_bounds(state, pos.x, pos.y))
    return NOT_FOUND;
  return state->map[pos.y][pos.x];
}

int
map_at (state_t *state, int x, int y)
{
  return state->map[y][x];
}

void
state_init (state_t *state, config_t *config)
{
  /* Set the window to be running */
  state->running = true;

  /* set the resolution to be the config resolution */
  state->resolution.x = (int) config->window_width;
  state->resolution.y = (int) config->window_height;

  /* frame-adjusted speeds */
  state->player.speed_adjusted     = 0.0f;
  state->player.rot_speed_adjusted = 0.0f;
  
  state->camera_plane = (vec2s) {  0.0f,  0.66f };

  /* set up the map */
  memcpy(state->map, MAP, MAP_WIDTH * MAP_HEIGHT * sizeof(int));
  state->map_size = (ivec2s) { MAP_WIDTH, MAP_HEIGHT };

  /* setup the player's location on the map */
  state->player.pos       = (vec2s) { 22.0f, 12.0f  };
  state->player.dir       = (vec2s) { -1.0f,  0.0f  };
  state->player.speed     = 5.0f;
  state->player.rot_speed = 3.0f;

  if (state->map[(int) state->player.pos.x][(int) state->player.pos.y] != 0)
    state->player.out_of_bounds = true;
  
  /* initialize all of the keys as false */
  for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    state->keys[i] = false;

  /* timer init */
  state->timer = (fpstimer_t) { 0 };

  /* fonts */
  for (uint8_t i = 0; i < TOTAL_FONTS; ++i)
    state->fonts[i] = NULL;
  
  if (!TTF_WasInit()) {
    log_error("TTF was not initialized prior to state initialiation, so fonts cannot be loaded!");
    exit(EXIT_FAILURE);
  }

  /* load the deja vu sans mono font */
  state->fonts[DEJAVU_SANS_MONO] = TTF_OpenFont(DEJAVU_MONO_FONT_PATH,
						config->debug_font_size * FONT_SCALAR);
  state->debug_font = state->fonts[DEJAVU_SANS_MONO];
}

void
state_close (state_t *state)
{
  
}

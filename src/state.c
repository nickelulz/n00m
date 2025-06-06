#include <stdint.h>
#include <string.h>
#include "lib_common.h"

#include "color.h"
#include "state.h"
#include "config.h"
#include "util.h"
#include "map.h"

static void
player_init(player_t *player, map_t *map)
{
  /* frame-adjusted speeds */
  player->speed_adjusted     = 0.0f;
  player->rot_speed_adjusted = 0.0f;

  /* setup the player's location on the map */
  player->pos       = (vec2s) { 22.0f, 14.0f  };
  player->dir       = (vec2s) { -1.0f,  0.0f  };
  player->speed     = 5.0f;
  player->rot_speed = 3.0f;

  if (map_at_fvec(map, player->pos) != MAP_OPEN)
    player->out_of_bounds = true;
}

void
state_init (state_t *state, config_t *config)
{
  /* Set the window to be running */
  state->running = true;

  /* set up the map */
  map_init(&state->map);
  
  /* set the resolution to be the config resolution */
  state->resolution.x = (int) config->window_width;
  state->resolution.y = (int) config->window_height;

  /* set the camera plane */
  state->camera_plane = (vec2s) {  0.0f,  0.66f };
  
  player_init(&state->player, &state->map);
  
  /* initialize all of the keys as false */
  for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    state->keys[i] = false;

  /* timer init */
  state->timer = (fpstimer_t) { 0 };

  // Allocate each texture dynamically
  for (int i = 0; i < 8; ++i) {
    state->textures[i] = malloc(sizeof(uint32_t) * TEXTURE_WIDTH * TEXTURE_HEIGHT);
    if (!state->textures[i]) {
      fprintf(stderr, "Failed to allocate memory for texture %d\n", i);
      exit(EXIT_FAILURE);
    }
  }

  // Populate texture data
  for (int x = 0; x < TEXTURE_WIDTH; ++x) {
    for (int y = 0; y < TEXTURE_HEIGHT; ++y) {
      int xorcolor = (x * 256 / TEXTURE_WIDTH) ^ (y * 256 / TEXTURE_HEIGHT);
      int ycolor   = y * 256 / TEXTURE_HEIGHT;
      int xycolor  = y * 128 / TEXTURE_HEIGHT + x * 128 / TEXTURE_WIDTH;
      int idx      = y * TEXTURE_WIDTH + x;

      // flat red texture with black cross
      state->textures[0][idx] = 65536 * 254 * (x != y && x != TEXTURE_WIDTH - y);

      // sloped greyscale
      state->textures[1][idx] = xycolor + 256 * xycolor + 65536 * xycolor;

      // sloped yellow gradient
      state->textures[2][idx] = 256 * xycolor + 65536 * xycolor;

      // xor greyscale
      state->textures[3][idx] = xorcolor + 256 * xorcolor + 65536 * xorcolor;

      // xor green
      state->textures[4][idx] = 256 * xorcolor;

      // red bricks
      state->textures[5][idx] = 65536 * 192 * (x % 16 && y % 16);

      // red gradient
      state->textures[6][idx] = 65536 * ycolor;

      // flat grey texture
      state->textures[7][idx] = 128 + 256 * 128 + 65536 * 128;
    }
  }
  
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

  /* set up the mouse */
  SDL_SetRelativeMouseMode(SDL_TRUE);
  state->mouse_sensitivity = 0.0025f;
}

void
state_close (state_t *state)
{
  map_close(&state->map);

  for (int i = 0; i < 8; ++i) {
    free(state->textures[i]);
    state->textures[i] = NULL;
  }
}

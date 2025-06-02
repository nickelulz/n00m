#ifndef STATE_HEADER
#define STATE_HEADER

#include "lib_common.h"
#include "config.h"
#include "player.h"
#include "map.h"

typedef struct _fpstimer {
  uint64_t ticks, frames, last_update_time;
  float tps, fps;
  bool infinite_fps;
} fpstimer_t;

/* fonts */
enum { DEJAVU_SANS_MONO, TOTAL_FONTS };

typedef struct _state {
  bool running;
  vec2s camera_plane;
  ivec2s map_size;
  ivec2s resolution;

  /* loaded fonts */
  TTF_Font *fonts[TOTAL_FONTS];
  TTF_Font *debug_font;
  int debug_line_y;

  /* map information */
  map_t map;

  /* controls and whatnot */
  bool keys[SDL_NUM_SCANCODES];
  ivec2s mouse_pos_delta;
  float mouse_sensitivity;
  
  player_t player;
  fpstimer_t timer;
} state_t;

void state_init  (state_t *state, config_t *config);
void state_close (state_t *state);

#endif

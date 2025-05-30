#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <cglm/cglm.h>

typedef struct _player {
  vec2s pos, dir;
  float speed, rot_speed;
  float speed_adjusted, rot_speed_adjusted;

  /* debug monitors */
  bool out_of_bounds;
  bool blocked[2];
} player_t;

#endif

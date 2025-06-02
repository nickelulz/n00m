#ifndef MAP_HEADER
#define MAP_HEADER

#include "lib_common.h"

/* common defines */
#define MAP_OPEN 0
#define NOT_FOUND -1

typedef struct _map {
  int *cells;
  ivec2s size;
} map_t;

SDL_Color map_get_color (int map_value);

void map_init  (map_t *map);
void map_close (map_t *map);

int map_at_vec  (map_t *map, ivec2s pos);
int map_at_fvec (map_t *map, vec2s pos);
int map_at      (map_t *map, int x, int y);
#endif

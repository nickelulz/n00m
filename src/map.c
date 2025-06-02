#include "lib_common.h"
#include "state.h"
#include "util.h"
#include "color.h"

/* this would probably be loaded in like a mapfile or something */

#define MAP_WIDTH  24
#define MAP_HEIGHT 24

static const
int MAP[MAP_WIDTH][MAP_HEIGHT]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,5,0,5,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

SDL_Color
map_get_color (int map_value)
{
  switch (map_value) {
  case 1: return COLOR_RED;
  case 2: return COLOR_GREEN;
  case 3: return COLOR_BLUE;
  case 4: return COLOR_ORANGE;
  default: return COLOR_WHITE;
  }
}

void
map_init (map_t *map)
{
  map->cells = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(int));
  memcpy(map->cells, MAP, MAP_WIDTH * MAP_HEIGHT * sizeof(int));
  map->size = (ivec2s){ MAP_WIDTH, MAP_HEIGHT };
}

void
map_close (map_t *map)
{
  free(map->cells);
  map->cells = NULL;
}

static bool
in_bounds (map_t *map, int x, int y)
{
    return x >= 0 && x < map->size.x &&
           y >= 0 && y < map->size.y;
}

int
map_at (map_t *map, int x, int y)
{
  if (!in_bounds(map, x, y))
    return NOT_FOUND;
  return map->cells[y * map->size.x + x];
}

int
map_at_vec (map_t *map, ivec2s pos)
{
  return map_at(map, pos.x, pos.y);
}

int
map_at_fvec (map_t *map, vec2s pos)
{
  return map_at_vec(map, VEC2_INT(pos));
}

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
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

SDL_Color
map_get_color (int map_value)
{
  switch (map_value) {
  case 0: return COLOR_BLACK;
  case 1: return COLOR_BROWN;
  case 2: return COLOR_NAVY;
  case 3: return COLOR_MAGENTA;
  case 4: return COLOR_PINK;
  case 5: return COLOR_LIME;
  case 6: return COLOR_DARK_GREEN;
  case 7: return COLOR_GREEN;
  default: return COLOR_CYAN;
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

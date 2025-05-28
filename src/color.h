#ifndef COLOR_HEADER
#define COLOR_HEADER

#include <SDL2/SDL.h>

#define EXPAND_COLOR(color) color.r, color.g, color.b, color.a

static const SDL_Color COLOR_BLACK  = { 0, 0, 0, 0 };
static const SDL_Color COLOR_WHITE  = { 255, 255, 255, 255 };
static const SDL_Color COLOR_ORANGE = { 255, 165, 0,   100 };
static const SDL_Color COLOR_RED    = { 255, 0,   0,   255 };
static const SDL_Color COLOR_GREEN  = { 0,   255, 0,   255 };
static const SDL_Color COLOR_BLUE   = { 0,   0,   255, 255 };
  
#endif

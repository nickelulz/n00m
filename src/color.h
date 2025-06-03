#ifndef COLOR_HEADER
#define COLOR_HEADER

#include <SDL2/SDL.h>

#define EXPAND_COLOR(color) color.r, color.g, color.b, color.a


static const SDL_Color COLOR_BLACK        = {   0,   0,   0,   255 };
static const SDL_Color COLOR_WHITE        = { 255, 255, 255, 255 };
static const SDL_Color COLOR_LIGHT_GRAY   = { 211, 211, 211,  20  };
static const SDL_Color COLOR_GRAY         = { 128, 128, 128, 255 };
static const SDL_Color COLOR_DARK_GRAY    = {  64,  64,  64, 255 };

static const SDL_Color COLOR_RED          = { 255,   0,   0, 255 };
static const SDL_Color COLOR_DARK_RED     = { 139,   0,   0, 255 };
static const SDL_Color COLOR_ORANGE       = { 255, 165,   0, 100 };
static const SDL_Color COLOR_YELLOW       = { 255, 255,   0, 200 };
static const SDL_Color COLOR_GOLD         = { 255, 215,   0, 255 };

static const SDL_Color COLOR_GREEN        = {   0, 255,   0, 255 };
static const SDL_Color COLOR_DARK_GREEN   = {   0, 100,   0, 255 };
static const SDL_Color COLOR_LIME         = {  50, 205,  50, 255 };
static const SDL_Color COLOR_CYAN         = {   0, 255, 255, 255 };
static const SDL_Color COLOR_TEAL         = {   0, 128, 128, 255 };

static const SDL_Color COLOR_BLUE         = {   0,   0, 255, 255 };
static const SDL_Color COLOR_DARK_BLUE    = {   0,   0, 139, 255 };
static const SDL_Color COLOR_NAVY         = {   0,   0, 128, 255 };
static const SDL_Color COLOR_PURPLE       = { 128,   0, 128, 255 };
static const SDL_Color COLOR_MAGENTA      = { 255,   0, 255, 255 };
static const SDL_Color COLOR_PINK         = { 255, 192, 203, 200 };

static const SDL_Color COLOR_BROWN        = { 139,  69,  19, 255 };
static const SDL_Color COLOR_BEIGE        = { 245, 245, 220, 255 };
static const SDL_Color COLOR_TAN          = { 210, 180, 140, 255 };
  
#endif

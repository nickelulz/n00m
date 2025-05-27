#include <stdlib.h>
#include <SDL2/SDL.h>

#include "log.h"
#include "config.h"
#include "graphics.h"

void
graphics_init (graphics_t *gfx, config_t *config)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    log_error("SDL could not be initialized!\n"
              "SDL Error: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
  /* Disable compositor bypass */
  if (!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0")) {
    log_error("SDL cannot disable compositor bypass!");
    exit(EXIT_FAILURE);
  }
#endif

  gfx->window = SDL_CreateWindow(config->window_title,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 config->window_width,
                                 config->window_height,
                                 SDL_WINDOW_SHOWN);

  if (!gfx->window) {
    log_error("Window could not be created!\n"
              "SDL Error: %s", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  gfx->renderer = SDL_CreateRenderer(gfx->window, -1,
                                     SDL_RENDERER_ACCELERATED);

  if (!gfx->renderer) {
    log_error("Renderer could not be created!\n"
              "SDL Error: %s", SDL_GetError());
    SDL_DestroyWindow(gfx->window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
}

void
graphics_close (graphics_t *gfx)
{
  SDL_DestroyRenderer(gfx->renderer);
  SDL_DestroyWindow(gfx->window);
  SDL_Quit();
}

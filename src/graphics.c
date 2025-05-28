#include "lib_common.h"

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

  int image_flags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF;
  int image_init  = IMG_Init(image_flags); 
  
  if ((image_init & image_flags) != image_flags) {
    log_error("SDL_image could not be initialized!\n"
              "SDL_image Error: %s", IMG_GetError());
    exit(EXIT_FAILURE);
  }

  if (TTF_Init() == -1) {
    log_error("SDL_TTF could not be initialized!\n"
              "SDL_TTF Error: %s", TTF_GetError());
  }
  
#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
  /* Disable compositor bypass */
  if (!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0")) {
    log_error("SDL cannot disable compositor bypass!");
    exit(EXIT_FAILURE);
  }
#endif

  gfx->window = SDL_CreateWindow(config->window_title,
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
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

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

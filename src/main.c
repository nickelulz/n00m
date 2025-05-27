#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cglm/cglm.h>
#include "log.h"

#include "color.h"
#include "config.h"
#include "graphics.h"
#include "state.h"

static void
loop (graphics_t *gfx, state_t *state)
{
  /* handle events */
  SDL_Event event;
  SDL_WaitEvent(&event);

  if (event.type == SDL_QUIT) {
    state->running = false;
  }

  /* Initialize the renderer color the background */
  SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(state->background_color));

  /* Clear screen */
  SDL_RenderClear(gfx->renderer);

  /* Draw a rectangle */
  SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(state->rectangle_color));
  SDL_RenderFillRect(gfx->renderer, &state->rectangle);

  /* Update the Screen */
  SDL_RenderPresent(gfx->renderer);
}

int
main (void)
{
  log_info("Initializing");
  
  config_t config;
  config_load(&config, "config.ini");

  graphics_t gfx;
  graphics_init(&gfx, &config);

  state_t state;
  state_init(&state, &config);

  log_info("Looping");
  while (state.running)
    loop(&gfx, &state);
  
  log_info("Closing");
  
  graphics_close(&gfx);
  config_close(&config);
}

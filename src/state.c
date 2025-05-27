#include <stdint.h>
#include <stdbool.h>

#include "color.h"
#include "state.h"
#include "config.h"

void
state_init(state_t *state, config_t *config)
{
  /* background color */
  state->background_color = COLOR_BLACK;
  
  /* set the rectangle and its color */
  uint16_t w = config->window_width;
  uint16_t h = config->window_height;

  state->rectangle = (SDL_Rect) {
    .w = w / 2,
    .h = h / 2,
    .x = w / 4,
    .y = h / 4
  };

  state->rectangle_color = COLOR_ORANGE;

  /* Set the window to be running */
  state->running = true;
}

void
state_close(state_t *state)
{
  
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <cglm/cglm.h>
#include "log.h"

#include "color.h"
#include "config.h"
#include "graphics.h"
#include "state.h"

static void
draw_current_frame (graphics_t *gfx, state_t *state)
{
  /* Get the current box of the map that we are in */
  player_t *player = &state->player;
  
  ivec2s resolution;
  if (!SDL_GetWindowSize(gfx->window, &resolution.x, &resolution.y)) {
    log_error("Could not get window dimensions!\n"
              "SDL Error: %s", SDL_GetError());
    state->running = false;
    return;
  }
  
  for (int x = 0; x < resolution.x; ++x) {
    float camera_x = 2 * x / ((float) resolution.x) - 1;
    vec2s ray_dir =
    {
      player->dir.x + state->camera_plane.x * camera_x,
      player->dir.y + state->camera_plane.y * camera_x
    };

    /* which box of the map we're in */
    ivec2s map_pos = {
      (int) player->dir.x,
      (int) player->dir.y
    };

    /* length of the ray from the current position to the next x or y-side */
    vec2s side_dist;

    const float INF = 100;
    
    /* length of the ray from one x or y-side to the next x or y side */
    vec2s delta_dist = {
      (ray_dir.x == 0) ? INF : (float) fabs(1 / ray_dir.x),
      (ray_dir.y == 0) ? INF : (float) fabs(1 / ray_dir.y)
    };
    
    float perp_wall_dist;

    /* what direction to step in x or y-direction (either +1 or -1) */
    ivec2s step;

    bool wall_hit,   /* was there a wall a hit? */
      side_wall_hit; /* was a north-south or east-west wall hit? */

    /* Calculate the step and initial side distance */    
    if (ray_dir.x < 0) {
      step.x = -1;
      side_dist.x = (player->pos.x - map_pos.x) * delta_dist.x;
    } else {
      step.x = 1;
      side_dist.x = (map_pos.x + 1.0 - player->pos.x) * delta_dist.x;
    }

    if (ray_dir.y < 0) {
      step.y = -1;
      side_dist.y = (player->pos.y - map_pos.y) * delta_dist.y;
    } else {
      step.y = 1;
      side_dist.y = (map_pos.y + 1.0 - player->pos.y) * delta_dist.y;
    }

    /* perform DDA */
    while (!wall_hit)
    {
      /* jump to the next map square, either in
         x-direction, or in y-direction */
      if (side_dist.x < side_dist.y) {
        side_dist.x += delta_dist.x;
        map_pos.x += step.x;
        side_wall_hit = false;
      } else {
        side_dist.y += delta_dist.y;
        map_pos.y += step.y;
        side_wall_hit = true;
      }

      /* check if ray has hit a wall */
      if (state->map[map_pos.x][map_pos.y] > 0)
        side_wall_hit = true;
    }

    if (side_wall_hit)
      perp_wall_dist = side_dist.x - delta_dist.x;
    else
      perp_wall_dist = side_dist.y - delta_dist.y;

    /* calculate height of line to draw on screen */
    int line_height = (int) (resolution.y / perp_wall_dist);

    /* calculate lowest and highest pixel to fill in current stripe */
    int draw_start = (resolution.y - line_height) / 2;
    if (draw_start < 0)
      draw_start = 0;

    int draw_end = (resolution.y + line_height) / 2;
    if (draw_end >= resolution.y)
      draw_end = resolution.y - 1;

    /* color in the wall based on its color */
    SDL_Color wall_color = get_map_color(state->map[map_pos.x][map_pos.y]);

    /* give x and y sides different brightness */
    if (side_wall_hit)
      wall_color.a = wall_color.a / 2;

    /* draw the vertical line */
    SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(wall_color));
    SDL_RenderDrawLine(gfx->renderer, x, draw_start, x, draw_end);
  }
}

static void
handle_events(SDL_Event event, state_t *state)
{
  if (event.type == SDL_QUIT)
    state.running = false;
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
  
  while (state.running) {
    SDL_Event event;
    SDL_WaitEvent(&event);

    handle_events(event, &state);
    
    /* Initialize the renderer color the background */
    SDL_SetRenderDrawColor(gfx.renderer, EXPAND_COLOR(COLOR_BLACK));
    SDL_RenderClear(gfx.renderer);

    /* draw the current frame */
    draw_current_frame(&gfx, &state);

    /* Update the Screen */
    SDL_RenderPresent(gfx.renderer);
  }
  
  log_info("Closing");
  
  graphics_close(&gfx);
  config_close(&config);
}

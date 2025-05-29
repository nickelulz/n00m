#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "lib_common.h"
#include <cglm/struct/vec2.h>

#include "color.h"
#include "config.h"
#include "graphics.h"
#include "state.h"

static SDL_Color
get_map_color (int map_value)
{
  switch (map_value) {
  case 1: return COLOR_RED;
  case 2: return COLOR_GREEN;
  case 3: return COLOR_BLUE;
  case 4: return COLOR_WHITE;
  default: return COLOR_ORANGE;
  }
}

static void
draw_current_frame (graphics_t *gfx, state_t *state)
{
  /* Get the current box of the map that we are in */
  player_t *player = &state->player;
  ivec2s resolution = state->resolution;
  
  for (int x = 0; x < resolution.x; ++x) {
    float camera_x = 2 * x / ((float) resolution.x) - 1;
    vec2s ray_dir =
    {
      player->dir.x + state->camera_plane.x * camera_x,
      player->dir.y + state->camera_plane.y * camera_x
    };
    
    /* which box of the map we're in */
    ivec2s map_pos =
    {
      (int) player->dir.x,
      (int) player->dir.y
    };

    /* length of the ray from the current position to the next x or y-side */
    vec2s side_dist;

    const float INF = 100;
    
    /* length of the ray from one x or y-side to the next x or y side */
    vec2s delta_dist =
    {
      (ray_dir.x == 0) ? INF : (float) fabs(1 / ray_dir.x),
      (ray_dir.y == 0) ? INF : (float) fabs(1 / ray_dir.y)
    };
    
    float perp_wall_dist;

    /* what direction to step in x or y-direction (either +1 or -1) */
    ivec2s step;

    bool wall_hit      = false, /* was there a wall a hit? */
         side_wall_hit = false; /* was a north-south or east-west wall hit? */

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
        wall_hit = true;
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
poll_events (state_t *state)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type) {
    case SDL_QUIT:
      state->running = false;
      break;
    case SDL_KEYDOWN:
      state->keys[event.key.keysym.scancode] = true;
      break;
    case SDL_KEYUP:
      state->keys[event.key.keysym.scancode] = false;
      break;
    }
  }
}

static void
handle_keys(state_t *state)
{
  player_t *player = &state->player;

  
  if (state->keys[SDL_SCANCODE_ESCAPE])
    state->running = false;

  const bool UP    = state->keys[SDL_SCANCODE_UP];
  const bool DOWN  = state->keys[SDL_SCANCODE_DOWN];
  const bool RIGHT = state->keys[SDL_SCANCODE_RIGHT];
  const bool LEFT  = state->keys[SDL_SCANCODE_LEFT];
  
  /* if both keys are pressed simultaneously, do nothing
     move forward or backward if no wall is present */
  if (!(UP && DOWN) && (UP || DOWN))
  {
    vec2s position_delta = glms_vec2_scale(player->dir, player->speed_adjusted);
    vec2s new_pos;
    
    /* calculate the new position based on adding or subtracting the change */
    if (UP)
      /* moving forward */
      new_pos = glms_vec2_add(player->pos, position_delta);
    else if (DOWN)
      /* moving backward */
      new_pos = glms_vec2_sub(player->pos, position_delta);

    ivec2s map_pos = { (int) round(new_pos.x), (int) round(new_pos.y) };
    
    /* set the new position if no wall is present */
    if (state->map[map_pos.x][map_pos.y] == MAP_OPEN)
      player->pos.x = new_pos.x;
    if (state->map[map_pos.x][map_pos.y] == MAP_OPEN)
      player->pos.y = new_pos.y;
  }
  
  /* if both are pressed, do nothing
     rightward rotation */
  if (!(RIGHT && LEFT) && (RIGHT || LEFT))
  {
    float rotation_speed = player->rot_speed_adjusted;

    if (LEFT)
      rotation_speed = -rotation_speed;
    
    /* calculate the new direction by multiplying by the rotation matrix */
    player->dir         = glms_vec2_rotate(player->dir,         rotation_speed);
    state->camera_plane = glms_vec2_rotate(state->camera_plane, rotation_speed);
  }
}

static void
debug_printf (graphics_t *gfx, state_t *state, const char *fmt, ...)
{
  char buffer[512];

  /* format the string using variable argument list */
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  /* configure the text's position (top right corner) */
  ivec2s text_pos = { DEBUG_TEXT_PADDING, state->debug_line_y };
  
  /* calculate the projected text size */
  int text_height;
  if (TTF_SizeText(state->debug_font, buffer, NULL, &text_height) < 0) {
    log_error("Unable to calculate projected debug text size! "
	      "Increasing debug line by 10 pixels.");
    text_height = 10; /* px */
  }

  state->debug_line_y += text_height / FONT_SCALAR;
  graphics_draw_text(gfx, state->debug_font, buffer,
		     COLOR_BLACK, COLOR_LIGHT_GRAY,
		     text_pos.x, text_pos.y, 2);
}

static void
draw_debug(graphics_t *gfx, state_t *state)
{
  state->debug_line_y = DEBUG_TEXT_PADDING;

  char fps_num[32];
  snprintf(fps_num, sizeof(fps_num), "%.1f", state->timer.fps);
  debug_printf(gfx, state, "FPS: %s", state->timer.infinite_fps ? "Infinite" : fps_num);
  debug_printf(gfx, state, "Position: %.1f, %.1f", state->player.pos.x, state->player.pos.y);
  debug_printf(gfx, state, "Direction: %.1f, %.1f", state->player.dir.x, state->player.dir.y);
  debug_printf(gfx, state, "In Bounds? %s", state->player.out_of_bounds ? "NO" : "YES");
}
  
int
main (void)
{
  log_info("Initializing");

  /* setup the random number generator */
  srand(time(0));
  
  /* note: DO NOT break the initialization order */
  
  config_t config;
  config_load(&config);

  graphics_t gfx;
  graphics_init(&gfx, &config);

  state_t state;
  state_init(&state, &config);

  log_info("Looping");
  
  while (state.running) {
    /* time current frame */
    state.timer.ticks = SDL_GetTicks();
    uint64_t frame_begin = SDL_GetPerformanceCounter();
    state.timer.last_update_time = frame_begin;

    /* poll the current window size */
    SDL_GetWindowSize(gfx.window,
		      &state.resolution.x,
		      &state.resolution.y);
    
    /* Poll for events  */
    poll_events(&state);
    handle_keys(&state);

    /* check if position is out of bounds */
    if (state.map[(int) state.player.pos.x][(int) state.player.pos.y] != MAP_OPEN)
      state.player.out_of_bounds = true;
    
    /* Initialize the renderer color the background */
    SDL_SetRenderDrawColor(gfx.renderer, EXPAND_COLOR(COLOR_BLACK));
    SDL_RenderClear(gfx.renderer);

    /* draw the current frame */
    draw_current_frame(&gfx, &state);
    draw_debug(&gfx, &state);

    /* Update the Screen */
    SDL_RenderPresent(gfx.renderer);

    /* time frame and calculate fps */
    uint64_t frame_end = SDL_GetPerformanceCounter();
    uint64_t frame_diff = frame_end - frame_begin;
    float frequency = (float) SDL_GetPerformanceFrequency();
    float seconds_elapsed = frame_diff / frequency;
    
    state.timer.fps = seconds_elapsed;
    if (state.timer.fps < 0.1)
      state.timer.infinite_fps = true;
    else
      state.timer.infinite_fps = false;

    /* adjust rotation and movement speeds */
    state.player.speed_adjusted     = seconds_elapsed * state.player.speed;
    state.player.rot_speed_adjusted = seconds_elapsed * state.player.rot_speed;
    
    /* increase frame counter */
    ++state.timer.frames;
  }
  
  log_info("Closing");
  
  graphics_close(&gfx);
  config_close(&config);
}

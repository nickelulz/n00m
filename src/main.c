#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "lib_common.h"
#include <float.h>

#include "color.h"
#include "config.h"
#include "graphics.h"
#include "state.h"

#define LOG_DEBUG_ONCE(fmt, ...) \
  { \
    static bool has_been_printed = false; \
    if (!has_been_printed) { \
      log_debug(fmt, __VA_ARGS__); \
      has_been_printed = true; \
    } \
  }

#define VEC2_INT(vec) \
  (ivec2s) { (int) vec.x, (int) vec.y }

#define VEC2_FLT(vec) \
  (vec2s) { (float) vec.x, (float) vec.y }

static SDL_Color
get_map_color (int map_value)
{
  switch (map_value) {
  case 1: return COLOR_RED;
  case 2: return COLOR_GREEN;
  case 3: return COLOR_BLUE;
  case 4: return COLOR_ORANGE;
  default: return COLOR_WHITE;
  }
}

static void
draw_current_frame (graphics_t *gfx, state_t *state)
{
  /* Get the current box of the map that we are in */
  player_t *player = &state->player;
  ivec2s resolution = state->resolution;
  
  for (int x = 0; x < resolution.x; ++x) {
    float camera_x = 2 * ((float) x  / resolution.x) - 1;

    vec2s ray_dir = glms_vec2_scale(state->camera_plane, camera_x);
    ray_dir = glms_vec2_add(ray_dir, player->dir);
    
    /* which box of the map we're in */
    ivec2s map_pos = VEC2_INT(player->pos);

    /* length of the ray from the current position to the next x or y-side */
    vec2s side_dist;

    const float INF = FLT_MAX;
    
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
      if (state->map[map_pos.y][map_pos.x] > 0)
        wall_hit = true;
    }

    if (side_wall_hit)
      perp_wall_dist = side_dist.y - delta_dist.y;
    else
      perp_wall_dist = side_dist.x - delta_dist.x;
    
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
    SDL_Color wall_color = get_map_color(state->map[map_pos.y][map_pos.x]);

    /* give x and y sides different brightness */
    if (side_wall_hit)
      wall_color.a = wall_color.a / 2;

    /* draw the vertical line */
    SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(wall_color));
    SDL_RenderDrawLine(gfx->renderer, x, draw_start, x, draw_end);
  }
}

static void
draw_mini_map (graphics_t *gfx, state_t *state)
{
  /* minimap background constants */
  static const uint8_t BOX_SIZE    = 5; /* width of each box */
  static const uint8_t PADDING     = 1; /* padding for the background */
  static const uint8_t MARGIN      = 2; /* the outer margin */
  static const uint8_t PLAYER_SIZE = 4; /* width/height of the player icon */ 

  /* set up the box sizing for the outer box */
  static const ivec2s full_minimap_size = {
    MAP_WIDTH  * BOX_SIZE + 2 * PADDING,
    MAP_HEIGHT * BOX_SIZE + 2 * PADDING
  };


  /* determine placement based on the sizing */
  ivec2s full_minimap_position = {
    state->resolution.x - MARGIN - full_minimap_size.x,
    MARGIN
  };

  /* draw the background */
  SDL_Rect bg_rect = {
    full_minimap_position.x, full_minimap_position.y,
    full_minimap_size.x,     full_minimap_size.y
  };
  
  SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(COLOR_WHITE));
  SDL_RenderFillRect(gfx->renderer, &bg_rect);

  ivec2s box_top_left = glms_ivec2_adds(full_minimap_position, PADDING);
  
  LOG_DEBUG_ONCE("full_minimap_position %d %d", full_minimap_position.x, full_minimap_position.y);
  LOG_DEBUG_ONCE("box_top_left %d %d", box_top_left.x, box_top_left.y);
  
  /* draw all of the boxes */
  for (int x = 0; x < MAP_WIDTH; ++x) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {      
      /* get the fill color from the position */
      int map_value = state->map[y][x];
      SDL_Color fill_color;
      if (map_value == MAP_OPEN)
	fill_color = COLOR_BLACK;
      else
	fill_color = get_map_color(map_value);

      /* calculate the fill position from the r,c value */
      SDL_Rect box = {
	box_top_left.x + BOX_SIZE * x,
	box_top_left.y + BOX_SIZE * y,
	BOX_SIZE, BOX_SIZE,
      };

      LOG_DEBUG_ONCE("box %d %d %d %d",
		     box.w, box.h, box.x, box.y);
      
      /* now draw it to the screen */
      SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(fill_color));
      SDL_RenderFillRect(gfx->renderer, &box);
    }
  }
  
  /* draw a tiny square to represent the player */
  ivec2s player_indicator_center = {
    box_top_left.x + (int) (BOX_SIZE * state->player.pos.x),
    box_top_left.y + (int) (BOX_SIZE * state->player.pos.y)
  };
  
  SDL_Rect player_indicator_bounds = {
    player_indicator_center.x - PLAYER_SIZE / 2,
    player_indicator_center.y - PLAYER_SIZE / 2,
    PLAYER_SIZE, PLAYER_SIZE
  };

  LOG_DEBUG_ONCE("player_indicator_bounds %d %d %d %d",
		 player_indicator_bounds.w,
		 player_indicator_bounds.h,
		 player_indicator_bounds.x,
		 player_indicator_bounds.y)
  
  SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(COLOR_LIGHT_GRAY));
  SDL_RenderFillRect(gfx->renderer, &player_indicator_bounds);

  /* draw a line to represent the view direction */
  static const int view_line_distance = 20;

  /* normalize the direction vector then scale by the distance */
  vec2s direction_vector;
  glm_vec2_normalize_to((float*) state->player.dir.raw, direction_vector.raw);
  direction_vector = glms_vec2_scale(direction_vector, view_line_distance);

  ivec2s direction_vector_end = glms_ivec2_add(VEC2_INT(direction_vector),
					       player_indicator_center);
  
  /* draw the vector to the screen */
  SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(COLOR_LIGHT_GRAY));
  SDL_RenderDrawLine(gfx->renderer,
		     player_indicator_center.x,
		     player_indicator_center.y,
		     direction_vector_end.x,
		     direction_vector_end.y);
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

  const bool UP    = state->keys[SDL_SCANCODE_UP]    || state->keys[SDL_SCANCODE_W];
  const bool DOWN  = state->keys[SDL_SCANCODE_DOWN]  || state->keys[SDL_SCANCODE_S];
  const bool RIGHT = state->keys[SDL_SCANCODE_RIGHT] || state->keys[SDL_SCANCODE_D];
  const bool LEFT  = state->keys[SDL_SCANCODE_LEFT]  || state->keys[SDL_SCANCODE_A];
  
  /* if both keys are pressed simultaneously, do nothing
     move forward or backward if no wall is present */
  if (UP ^ DOWN)
  {
    /* calculate the new position based on adding or subtracting the change */
    float dir_sign = UP ? 1.0f : -1.0f;
    vec2s position_delta = glms_vec2_scale(player->dir, player->speed_adjusted * dir_sign);
    vec2s pos_change = glms_vec2_add(player->pos, position_delta);
    ivec2s new_pos = VEC2_INT(pos_change);

    LOG_DEBUG_ONCE("new_pos %d %d", new_pos.x, new_pos.y);
    
    /* set the new position if no wall is present */
    ivec2s x_check = { new_pos.x, (int) player->pos.y };
    if (map_at(state, x_check.x, x_check.y) == MAP_OPEN)
      player->pos.x = new_pos.x;
    else
      player->blocked[0] = true;

    // Check y movement
    ivec2s y_check = { (int)player->pos.x, (int)new_pos.y };
    if (map_at(state, y_check.x, y_check.y) == MAP_OPEN)
      player->pos.y = new_pos.y;
    else
      player->blocked[1] = true;
  }
  
  /* if both are pressed, do nothing
     rightward rotation */
  if (RIGHT ^ LEFT)
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
  debug_printf(gfx, state, "Speed (raw): %.1f", state->player.speed);
  debug_printf(gfx, state, "Speed (adjusted): %f", state->player.speed_adjusted);
  debug_printf(gfx, state, "Rotation Speed (raw): %.1f", state->player.rot_speed);
  debug_printf(gfx, state, "Rotation Speed (adjusted): %.1f", state->player.rot_speed_adjusted);
  debug_printf(gfx, state, "In Bounds? %s", state->player.out_of_bounds ? "NO" : "YES");
  debug_printf(gfx, state, "Blocked? %s %s",
	       state->player.blocked[0] ? "YES" : "NO",
	       state->player.blocked[1] ? "YES" : "NO");
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
    else
      state.player.out_of_bounds = false;
    
    /* Initialize the renderer color the background */
    SDL_SetRenderDrawColor(gfx.renderer, EXPAND_COLOR(COLOR_BLACK));
    SDL_RenderClear(gfx.renderer);

    /* draw the current frame */
    draw_current_frame(&gfx, &state);
    draw_mini_map(&gfx, &state);
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

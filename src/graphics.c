#include "lib_common.h"

#include "color.h"
#include "log.h"
#include "config.h"
#include "graphics.h"

void
graphics_draw_pixel_buffer(graphics_t *gfx, ivec2s resolution, uint32_t *pixels)
{
  void *texture_pixels;
  int pitch;

  /* lock the texture for writing */
  if (SDL_LockTexture(gfx->screen_texture, NULL, &texture_pixels, &pitch) == 0) {
    /* copy the buffer into the texture */
    for (int y = 0; y < resolution.y; ++y)
      memcpy((uint8_t*) texture_pixels + y * pitch,
             pixels + y * resolution.x,
             resolution.x * sizeof(uint32_t));
    SDL_UnlockTexture(gfx->screen_texture);
  }

  SDL_RenderClear(gfx->renderer);
  SDL_RenderCopy(gfx->renderer, gfx->screen_texture, NULL, NULL);
}
  
void
graphics_draw_text (graphics_t *gfx, TTF_Font *font, const char *text,
		    SDL_Color text_color, SDL_Color background_color,
		    int x, int y, int padding)
{
  /* render the text to a surface */
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, text_color);
  if (!surface) {
    log_error("Could not draw text to surface: \"%s\"\n"
	      "SDL_ttf error: %s", text, TTF_GetError());
    return;
  }

  /* render the surface to a texture */
  SDL_Texture *texture = SDL_CreateTextureFromSurface(gfx->renderer, surface);
  if (!texture) {
    SDL_FreeSurface(surface);
    log_error("Could not convert surface to texture to draw text: \"%s\"\n"
	      "SDL_Error: %s", text, SDL_GetError());
    return;
  }

  /* calculate the bounding boxes */
  SDL_Rect dst_rect = { x, y, surface->w / FONT_SCALAR, surface->h / FONT_SCALAR };


  SDL_Rect bg_rect = {
    x - padding, y - padding,
    surface->w / FONT_SCALAR + 2 * padding,
    surface->h / FONT_SCALAR + 2 * padding
  };

  /* draw background highlight */
  SDL_SetRenderDrawColor(gfx->renderer, EXPAND_COLOR(background_color));
  SDL_RenderFillRect(gfx->renderer, &bg_rect);

  /* draw the text */
  SDL_RenderCopy(gfx->renderer, texture, NULL, &dst_rect);

  /* free the surface and texture */
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void
graphics_init (graphics_t *gfx, config_t *config)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    log_fatal("SDL could not be initialized!\n"
              "SDL Error: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  int image_flags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF;
  int image_init  = IMG_Init(image_flags); 
  
  if ((image_init & image_flags) != image_flags) {
    log_fatal("SDL_image could not be initialized!\n"
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
    log_fatal("SDL cannot disable compositor bypass!");
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
    log_fatal("Window could not be created!\n"
              "SDL Error: %s", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  gfx->renderer = SDL_CreateRenderer(gfx->window, -1,
                                     SDL_RENDERER_PRESENTVSYNC);

  if (!gfx->renderer) {
    log_fatal("Renderer could not be created!\n"
              "SDL Error: %s", SDL_GetError());
    SDL_DestroyWindow(gfx->window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  gfx->screen_texture = SDL_CreateTexture(gfx->renderer,
                                          SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_STREAMING,
                                          config->window_width,
                                          config->window_height);
}

void
graphics_close (graphics_t *gfx)
{
  SDL_DestroyTexture(gfx->screen_texture);
  SDL_DestroyRenderer(gfx->renderer);
  SDL_DestroyWindow(gfx->window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

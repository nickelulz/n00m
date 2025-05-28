#include "lib_common.h"
#include "config.h"

static const uint16_t DIMENSION_MAX  = 10000;
static const uint8_t  DEBUG_FONT_MAX = 100;

void
config_load (config_t *config, const char *config_filepath)
{
  ini_t *config_file = ini_load(config_filepath);
  if (!config_file) {
    log_error("Unable to load config file %s: %s",
              config_filepath, strerror(errno));
    exit(EXIT_FAILURE);
  }
  
  char *temp_title_buffer;
  if (ini_sget(config_file, "window", "title", NULL, &temp_title_buffer))
  {
    config->window_title = strdup(temp_title_buffer);
  }

  /* ensuring there are no memory leaks... hopefully */
  int64_t window_width, window_height;
  
  ini_sget(config_file, "window", "width",  "%lld", &window_width);
  ini_sget(config_file, "window", "height", "%lld", &window_height);

  if (window_width > DIMENSION_MAX || window_height > DIMENSION_MAX) {
    log_warn("[Config] Input window resolution (%lld x %lld) too large! "
	     "Both dimensions must be within [1, 10,000]. "
	     "Switching to default resolution, 400 x 400.",
	     window_width, window_height);
    config->window_width  = 400;
    config->window_height = 400;
  }
  else if (window_width <= 0 || window_height <= 0) {
    log_warn("[Config] Input window resolution (%lld x %lld) too small! "
	     "Both dimensions must be within [1, 10,000]. "
	     "Switching to default resolution, 400 x 400.",
	     window_width, window_height);
    config->window_width  = 400;
    config->window_height = 400;    
  }
  else {
    config->window_width  = (uint16_t) window_width;
    config->window_height = (uint16_t) window_height;
  }

  /* font sizing */
  int64_t debug_font_size;
  ini_sget(config_file, "debug", "fontsize", "%lld", &debug_font_size);

  if (debug_font_size > DEBUG_FONT_MAX || debug_font_size <= 0) {
    log_warn("[Config] Debug font sizing is too small and/or too large! "
	     "Input value %lld must be within [1, 100]. "
	     "Switching to default font size, 14.",
	     debug_font_size);
    config->debug_font_size = 14;
  }
  else {
    config->debug_font_size = (uint8_t) debug_font_size;
  }
  
  ini_free(config_file);

  log_debug("[Config] Window Title: %s", config->window_title);
  log_debug("[Config] Window Dimensions: %d x %d",
           config->window_width, config->window_height);
}

void
config_close (config_t *config)
{
  if (!config)
    return;
  if (config->window_title) {
    free(config->window_title);
    config->window_title = NULL;
  }
}

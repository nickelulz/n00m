#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ini.h"
#include "log.h"
#include "config.h"

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

  ini_sget(config_file, "window", "width",  "%i", &config->window_width);
  ini_sget(config_file, "window", "height", "%i", &config->window_height);
  
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

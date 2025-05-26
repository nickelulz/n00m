#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "ini.h"
#include "log.h"
#include "config.h"

void
load_config(config_t *config, const char *config_filepath)
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
  
  ini_sget(config_file, "window", "width",  "%d", &config->window_width);
  ini_sget(config_file, "window", "height", "%d", &config->window_height);
  
  ini_free(config_file);
}

void
config_close(config_t *config)
{
  if (!config)
    return;
  if (config->title) {
    free(config->title);
    config->title = NULL;
  }
}

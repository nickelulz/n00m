#include "lib_common.h"
#include "util.h"
#include "config.h"

static void
random_window_title_motd (char *buffer)
{
  const char *WINDOW_MOTDS[] = {
    "a nickelulzian d00mstyle renderer",
    "a dickelulzian n00mstyle renderer",
    "a pickelulzian c00mstyle renderer",
    "is this thing on?"
  };

  snprintf(buffer, 512, "n00m: %s", WINDOW_MOTDS[randint(0, 4)]);
}

void
config_load (config_t *config)
{
  config->window_width  = 1200;
  config->window_height = 800;
  random_window_title_motd(config->window_title);
  
  config->debug_font_size = 15;
}

void
config_close (config_t *config)
{}

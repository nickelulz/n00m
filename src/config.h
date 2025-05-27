#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include <stdint.h>

typedef struct _config {
  uint32_t window_height, window_width;
  char *window_title;
} config_t;

void config_load (config_t *config, const char *config_filepath);
void config_close (config_t *config);

#endif

#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include <stdint.h>

typedef struct _config {
  uint32_t window_height, window_width;
  uint8_t debug_font_size;
  char window_title[512];
} config_t;

void config_load (config_t *config);
void config_close (config_t *config);

/* constants and resource paths */
#define DEJAVU_MONO_FONT_PATH "./res/DejaVuSansMono.ttf"
#define CONFIG_FILEPATH       "./config.ini"
#define DEBUG_TEXT_PADDING    5 /* pixels */
#define DEBUG_LINE_SPACING    4 /* pixels */
#define FONT_SCALAR           8

#endif

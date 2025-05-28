/*
 * COMMON LIBRARIES INCLUDE HEADER
 *
 * Purpose: All common libraries within the project,
 *          not including local headers and standard
 *          library headers for the sake of ease of
 *          typing (except for stdbool and stdint).
 */

#ifndef LIB_COMMON_INCLUDE_HEADER
#define LIB_COMMON_INCLUDE_HEADER

/* Common Library */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>

/* SDL */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define CGLM_DEFINE_STRUCT
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include "log.h"
#include "ini.h"

#endif

#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include "lib_common.h"

inline static int randint(int min, int max)
{
  return (int) (rand() % (max - min + 1)) + min;
}

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

#endif

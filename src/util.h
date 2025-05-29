#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include "lib_common.h"

inline static int randint(int min, int max)
{
  return (int) (rand() % (max - min + 1)) + min;
}

#endif

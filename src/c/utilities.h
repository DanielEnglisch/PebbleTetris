#if !defined UTILITIES_H
#define UTILITIES_H

#include <pebble.h>

#include "./assert.h"

#undef APP_LOG_DEBUG
#define APP_LOG_DEBUG(fmt, args...) \
  APP_LOG(APP_LOG_LEVEL_DEBUG, fmt, ##args)

#undef APP_LOG_ERROR
#define APP_LOG_ERROR(fmt, args...) \
  APP_LOG(APP_LOG_LEVEL_ERROR, fmt, ##args)

#undef STRINGIZE
#define STRINGIZE(x) #x

static inline int ceil_div(int const a, int const b)
{
  assert(0 <= a);
  assert(0 < b);

  return (a + b - 1) / b;
}

static inline int floor_div_mult(int const a, int const b)
{
  assert(0 <= a);
  assert(0 < b);

  return (a / b) * b;
}

static inline int random_range(int const low, int const high)
{
  static bool initialized = false;

  if (!initialized)
  {
    srand(time(0));
    initialized = true;
  }

  assert(low <= high);
  return rand() % (high - low + 1) + low;
}

static inline int random(int const range)
{
  return random_range(0, range - 1);
}

#endif

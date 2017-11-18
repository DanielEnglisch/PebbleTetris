//       $Id: utilities.h 36046 2017-11-11 15:09:28Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/bin/cepheiden/vocational/teaching/SE/SWO3/2017-WS/Ue/src/06/tetris-template/src/utilities.h $
// $Revision: 36046 $
//     $Date: 2017-11-11 16:09:28 +0100 (Sa., 11 Nov 2017) $
//   Creator: erik.pitzer<AT>fh-hagenberg.at
//            johann.heinzelreiter<AT>fh-hagenberg.at
//            peter.kulczycki<AT>fh-hagenberg.at
//   $Author: p20068 $

#if !defined UTILITIES_H
#define      UTILITIES_H

#include <pebble.h>

#include "./assert.h"

// -------------------------------------------------------------------------------------------------

#undef  APP_LOG_DEBUG
#define APP_LOG_DEBUG(fmt, args...) \
   APP_LOG (APP_LOG_LEVEL_DEBUG, fmt, ##args)

#undef  APP_LOG_ERROR
#define APP_LOG_ERROR(fmt, args...) \
   APP_LOG (APP_LOG_LEVEL_ERROR, fmt, ##args)

#undef  STRINGIZE
#define STRINGIZE(x) #x

// -------------------------------------------------------------------------------------------------

static inline int ceil_div (int const a, int const b) {
   assert (0 <= a);
   assert (0 <  b);

   return (a + b - 1) / b;
}

static inline int floor_div_mult (int const a, int const b) {
   assert (0 <= a);
   assert (0 <  b);

   return (a / b) * b;
}

// -------------------------------------------------------------------------------------------------

static inline int random_range (int const low, int const high) {
   static bool initialized = false;

   if (!initialized) {
      srand (time (0)); initialized = true;
   }

   assert (low <= high); return rand () % (high - low + 1) + low;
}

static inline int random (int const range) {
   return random_range (0, range - 1);
}

// -------------------------------------------------------------------------------------------------

#endif   // UTILITIES_H

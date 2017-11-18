//       $Id: assert.h 36046 2017-11-11 15:09:28Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/bin/cepheiden/vocational/teaching/SE/SWO3/2017-WS/Ue/src/06/tetris-template/src/assert.h $
// $Revision: 36046 $
//     $Date: 2017-11-11 16:09:28 +0100 (Sa., 11 Nov 2017) $
//   Creator: erik.pitzer<AT>fh-hagenberg.at
//            johann.heinzelreiter<AT>fh-hagenberg.at
//            peter.kulczycki<AT>fh-hagenberg.at
//   $Author: p20068 $

#if !defined ASSERT_H
#define      ASSERT_H

// -------------------------------------------------------------------------------------------------

#undef assert

#ifdef NDEBUG
   #define assert(x) ((void) 0);
#else
   #define assert(x)                                              \
      if (!(x)) {                                                 \
         APP_LOG_ERROR ("Assertion '%s' failed!", STRINGIZE (x)); \
         window_stack_pop_all (false);                            \
      }
#endif

// -------------------------------------------------------------------------------------------------

#endif   // ASSERT_H

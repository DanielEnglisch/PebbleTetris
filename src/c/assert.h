#if !defined ASSERT_H
#define ASSERT_H

#undef assert

#ifdef NDEBUG
#define assert(x) ((void)0);
#else
#define assert(x)                                                  \
      if (!(x))                                                    \
      {                                                            \
            APP_LOG_ERROR("Assertion '%s' failed!", STRINGIZE(x)); \
            window_stack_pop_all(false);                           \
      }
#endif

#endif

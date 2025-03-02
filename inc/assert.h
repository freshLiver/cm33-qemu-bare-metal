#ifndef __MY_ASSERT_H__
#define __MY_ASSERT_H__

#include "compiler_tricks.h"
#include "stdbool.h"

static inline void assert(bool expr) {
  if (!expr)
    while (1)
      ;
}

#define static_assert(expr)                                                 \
  struct cat(static_assert_, __LINE__) {                                    \
    char : -!(expr);                                                        \
    /* :0 is only allowed for nameless field, with special meaning "start a \
     * new byte" */                                                         \
  }

#endif /* __MY_ASSERT_H__ */
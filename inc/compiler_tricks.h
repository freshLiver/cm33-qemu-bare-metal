#ifndef __MY_COMPILER_TRICKS_H__
#define __MY_COMPILER_TRICKS_H__

#include "stdint.h"

#define _str(text) #text
#define str(text) _str(text)

#define _cat(a, b) a##b
#define cat(a, b) _cat(a, b)

char cat(test, test);

#define offset_of(mem, st) (uintptr_t) & ((st*)0)->mem

#define static_assert(expr)                                                 \
  struct cat(static_assert_, __LINE__) {                                    \
    char : -!(expr);                                                        \
    /* :0 is only allowed for nameless field, with special meaning "start a \
     * new byte" */                                                         \
  }

#endif /* __MY_COMPILER_TRICKS_H__ */
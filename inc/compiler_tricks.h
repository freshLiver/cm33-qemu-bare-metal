#ifndef __MY_COMPILER_TRICKS_H__
#define __MY_COMPILER_TRICKS_H__

#include "stdint.h"

#define _str(text) #text
#define str(text) _str(text)

#define _cat(a, b) a##b
#define cat(a, b) _cat(a, b)

#define offset_of(mem, st) (uintptr_t) & ((st*)0)->mem

#endif /* __MY_COMPILER_TRICKS_H__ */
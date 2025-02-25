#include <stdbool.h>
#include <stdint.h>

extern void SystemInit(void);

static inline void assert(bool expr) {
  extern void (*__vector_table__[16])(void);
  if (!expr)
    __vector_table__[3]();
}

static int g_value_0;
static int g_value_10 = 10;
static const int g_const_value_100 = 100;

int main(void) {
  SystemInit();

  assert(g_value_0 == 0);
  assert(g_value_10 == 10);
  assert(g_const_value_100 == 100);

  while (1)
    ;

  return 0;
}

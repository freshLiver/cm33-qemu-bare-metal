#include "stdbool.h"
#include "stdint.h"

extern void SystemInit(void);

static inline void assert(bool expr) {
  extern void (*__vector_table__[16])(void);
  if (!expr)
    __vector_table__[3]();
}

static int bss_data;
static int data_val_10 = 10;
static const int rodata_val_100 = 100;

int main(void) {
  SystemInit();

  assert(bss_data == 0);
  assert((bss_data = 123) == 123);
  assert(data_val_10 == 10);
  assert((data_val_10 = 12345) == 12345);
  assert(rodata_val_100 == 100);

  while (1)
    ;

  return 0;
}

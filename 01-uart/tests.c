#include "assert.h"

static int bss_data;
static int data_val_10 = 10;
static const int rodata_val_100 = 100;

void TEST_DATA() {
  assert(bss_data == 0);
  assert((bss_data = 123) == 123);
  assert(data_val_10 == 10);
  assert((data_val_10 = 12345) == 12345);
  assert(rodata_val_100 == 100);
}
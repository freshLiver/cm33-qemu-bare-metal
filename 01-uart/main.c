#include "uart.h"

extern void SystemInit(void);
extern void TEST_DATA();

int main(void) {
  SystemInit();
  TEST_DATA();

  UartInit();

  UartPrint("start echoing messages from serial 1...\n");
  char buf[101];
  while (1) {
    buf[100] = 0;

    UartGetline(buf, 100);
    UartPrint(buf);
  }

  while (1)
    ;

  return 0;
}

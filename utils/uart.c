#include "assert.h"

#include "uart.h"

volatile uart_t* uart0 = (void*)UART0_REG_BASE;
volatile uart_t* uart1 = (void*)UART1_REG_BASE;

static_assert(offset_of(data_reg, uart_t) == 0x00);
static_assert(offset_of(state_reg, uart_t) == 0x04);
static_assert(offset_of(ctrl_reg, uart_t) == 0x08);
static_assert(offset_of(intstat_reg, uart_t) == 0x0C);
static_assert(offset_of(bauddiv, uart_t) == 0x10);

void UartInit() {
  /* The doc said:
   * > You must program the baud rate divider register before enabling the UART.
   * > For example, if the PCLK is running at 12MHz, and the required baud rate
   * > is 9600, program the baud rate divider register as 12,000,000/9600 =
   * > 1250.
   *
   * And, according to the source code, the PCLK is 20MHz:
   * > ```c
   * > qdev_prop_set_uint32(DEVICE(uart), "pclk-frq", mmc->apb_periph_frq);
   * > ...
   * > ...
   * > mmc->sysclk_frq = 20 * 1000 * 1000;
   * > mmc->apb_periph_frq = mmc->sysclk_frq;
   * > ```
   * so, the bauddiv should be set to 20,000,000/BAUD_RATE
   *
   * However, the bauddiv seems doesn't affect the I/O, so keep default here...
   */
  uart0->ctrl_reg = uart0->state_reg = uart0->intstat_reg = 0;
  uart0->ctrl.TX_EN = 1;

  uart1->ctrl_reg = uart1->state_reg = uart1->intstat_reg = 0;
  uart1->ctrl.RX_EN = 1;
}

void UartPutc(char c) {
  while (uart0->state.TXFULL)
    ;
  uart0->data = c;
}

void UartPrint(const char* msg) {
  while (*msg) {
    UartPutc(*msg++);
  }
}

char UartGetc() {
  while (!uart1->state.RXFULL)
    ;
  return uart1->data;
}

void UartGetline(char* msg, int len) {
  for (int i = 0; i < len; ++i, ++msg) {
    *msg = UartGetc();
    *msg &= -!(*msg == '\n');
    if (!*msg)
      break;
  }
}
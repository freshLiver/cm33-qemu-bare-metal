#include "assert.h"
#include "stdint.h"

extern void SystemInit(void);

/* The offsets of uart regs are defined in qemu/hw/char/cmsdk-apb-uart.{c,h}
 * Also check the official docs for the detailed usage and meaning of each reg:
 * https://developer.arm.com/documentation/ddi0479/c/apb-components/apb-uart */
typedef struct CMSDK_UART_REGS {
  union {
    struct {
      uint8_t data;
    };
    uint32_t data_reg;
  };

  union {
    struct {
      uint8_t TXFULL : 1;
      uint8_t RXFULL : 1;
      uint8_t TXOVERRUN : 1;
      uint8_t RXOVERRUN : 1;
      uint8_t reserved[3];
    } state;
    uint32_t state_reg;
  };

  union {
    struct {
      uint8_t TX_EN : 1;
      uint8_t RX_EN : 1;
      uint8_t TX_INTEN : 1;
      uint8_t RX_INTEN : 1;
      uint8_t TXO_INTEN : 1;
      uint8_t RXO_INTEN : 1;
      uint8_t HSTEST : 1;
      uint8_t reserved[3];
    } ctrl;
    uint32_t ctrl_reg;
  };
  union {
    struct {
      uint8_t TX;
      uint8_t RX;
      uint8_t TXO;
      uint8_t RXO;
    } intstat;
    uint32_t intstat_reg;
  };

  uint32_t bauddiv;
} uart_t;

static_assert(offset_of(data_reg, uart_t) == 0x00);
static_assert(offset_of(state_reg, uart_t) == 0x04);
static_assert(offset_of(ctrl_reg, uart_t) == 0x08);
static_assert(offset_of(intstat_reg, uart_t) == 0x0C);
static_assert(offset_of(bauddiv, uart_t) == 0x10);

#define UART0_REG_BASE 0x40200000  // -serial mon:stdio
#define UART1_REG_BASE 0x40201000  // -serial pty

volatile uart_t* uart0 = (void*)UART0_REG_BASE;
volatile uart_t* uart1 = (void*)UART1_REG_BASE;

static inline void initUART() {
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

static inline void uart_putc(char c) {
  while (uart0->state.TXFULL)
    ;
  uart0->data = c;
}

static inline void print(const char* msg) {
  while (*msg) {
    uart_putc(*msg++);
  }
}

static inline char uart_getc() {
  while (!uart1->state.RXFULL)
    ;
  return uart1->data;
}

static inline void getline(char* msg, int len) {
  for (int i = 0; i < len; ++i, ++msg) {
    *msg = uart_getc();
    *msg &= -!(*msg == '\n');
    if (!*msg)
      break;
  }
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

  initUART();

  print("start echoing messages from serial 1...\n");
  char buf[101];
  while (1) {
    buf[100] = 0;

    getline(buf, 100);
    print(buf);
  }

  while (1)
    ;

  return 0;
}

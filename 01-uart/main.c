#include "stdbool.h"
#include "stdint.h"

extern void SystemInit(void);

static inline void assert(bool expr) {
  extern void (*__vector_table__[16])(void);
  if (!expr)
    __vector_table__[3]();
}

// FIXME: add static asserts

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
    } int_state;
    uint32_t int_state_reg;
  };

  uint32_t bauddiv;
} uart_t;

#define UART0_REG_BASE 0x50200000
volatile uart_t* uart = (void*)UART0_REG_BASE;

static inline void initUART() {
  /* The doc said:
   * > You must program the baud rate divider register before enabling the UART.
   * > For example, if the PCLK is running at 12MHz, and the required baud rate
   * > is 9600, program the baud rate divider register as 12,000,000/9600 =
   * > 1250.
   */
  uart->ctrl_reg = uart->state_reg = uart->ctrl_reg = uart->int_state_reg = 0;
  uart->ctrl.TX_EN = 1;
}

static inline void print(const char* msg) {
  volatile uint8_t* uart_data = &uart->data;
  while (*msg) {
    *uart_data = *msg++;
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
  print("hello !!!!!");
  while (1)
    ;

  return 0;
}

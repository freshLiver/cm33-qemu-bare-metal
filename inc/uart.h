#ifndef __MY_UART_H__
#define __MY_UART_H__

#include "stdint.h"

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

#define UART0_REG_BASE 0x40200000  // -serial mon:stdio
#define UART1_REG_BASE 0x40201000  // -serial pty

extern volatile uart_t* uart0;
extern volatile uart_t* uart1;

void UartInit();

void UartPutc(char c);
char UartGetc();

void UartPrint(const char* msg);
void UartGetline(char* msg, int len);

#endif /* __MY_UART_H__ */
#include "stdint.h"

/*
 * this file is used to manage the vector table and the exceptions
 */

void DefaultHandler(void) {
  while (1)
    ;
}

extern int main(void);
void ResetHandler(void) {
  // copy .data from ROM to RAM
  extern uint8_t _data, _edata, _data_rom;
  for (uint8_t *src = &_data_rom, *dst = &_data; dst < &_edata; ++src, ++dst)
    *dst = *src;

  // clear .bss section
  extern char _bstart, _bend;
  for (char* p = &_bstart; p < (&_bend); ++p)
    *p = 0;  // we must clear the bss section

  main();

  while (1)
    ;  // not expected to be here
}

/* we need to specify the section of the vector table to .vectors (check
 * the linker script)
 *
 * Check the CM33 dgug 2.3.4 for the layout of vector table */
typedef void (*isr_t)(void);
__attribute__((section(".vectors"))) isr_t __vector_table__[16] = {
    (isr_t)0x28400000,  // the top addr of the sram, check the MEMORY config
    ResetHandler,       // Reset Handler
    0,
    DefaultHandler,  // Hard Fault
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

void SystemInit() {
  /*
   * Update the SCB->VTOR (0xE000ED08) to our vector table
   */
  isr_t** SCB_VTOR = (isr_t**)0xE000ED08;
  *SCB_VTOR = __vector_table__;
}

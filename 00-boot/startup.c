#include <stdint.h>

/*
 * this file is used to manage the vector table and the exceptions
 */

void DefaultHandler(void) {
  while (1)
    ;
}

extern int main(void);
void ResetHandler(void) {
  extern char __bss_start, __bss_end;
  for (char* addr = &__bss_start; addr < (&__bss_end); ++addr)
    *addr = 0;  // we must clear the bss section

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

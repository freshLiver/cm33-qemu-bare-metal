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
  main();
  while (1)
    ;
}

/* we need to specify the section of the vector table to .vectors (check
 * the linker script)
 *
 * Check the CM33 dgug 2.3.4 for the layout of vector table */
extern uint32_t __StackTop;

__attribute__((section(".vectors"))) uint32_t __vector_table__[16] = {
    0x28200000,              // FIXME: init SP
    (uint32_t)ResetHandler,  // Reset Handler
    0,
    (uint32_t)DefaultHandler,  // Hard Fault
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
  uint32_t* SCB_VTOR = (uint32_t*)0xE000ED08;
  *SCB_VTOR = (uint32_t)&__vector_table__;
}

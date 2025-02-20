MEMORY
{
    /* the CM33 dgug 2.3.4 said:
     * > The initial values of VTOR_S and VTOR_NS are system design
     * > specific.
     * check the mmc->init_svtor for the base addr */
    ROM (rx)  : ORIGIN = 0x10000000, LENGTH = 128k

    /* check CM33 dgug 2.2.1 for the base addr of SRAM;
     * and check the an505_raminfo for the length */
    RAM (rwx) : ORIGIN = 0x28000000, LENGTH = 2m
}

SECTIONS
{
    /* the ROM (flash) consists of vector tables, codes (.text), and 
     * initialized data (.data) */
    .text :
    {
        *(.vectors) /* make sure the vector table begins at init_svtor */
        *(.text)
        *(.data)
    } > ROM 

    /* the uninit data, and zero-init data (.bss) should be place to RAM */
    zeros :
    {
        *(.bss)
    } > RAM


    /* define the stack:
     *
     * the stack top addr is defined at vector table[0]
     * teh size is simply set to 4k
     * */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __STACK_SIZE = 0x1000;
    __StackLimit = __StackTop - __STACK_SIZE;
}

/* the entry point of the executable */
ENTRY(ResetHandler)

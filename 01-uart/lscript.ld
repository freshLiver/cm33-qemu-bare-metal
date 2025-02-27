MEMORY
{
    /* the CM33 dgug 2.3.4 said:
     * > The initial values of VTOR_S and VTOR_NS are system design specific.
     * check the mmc->init_svtor for the base addr */
    ROM (rx)  : ORIGIN = 0x10000000, LENGTH = 128k

    /* check CM33 dgug 2.2.1 defines the base addr range for RAM, however the
     * actual valid addrs of target device SRAM are defined in an505_raminfo.
     * Choose one of the memory region as RAM; don't forget to update the
     * __vector_table__[0] */
    /* RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 4m */
    /* RAM (rwx) : ORIGIN = 0x28000000, LENGTH = 2m */
    RAM (rwx) : ORIGIN = 0x28200000, LENGTH = 2m
}

SECTIONS
{
    /* the ROM (flash) consists of vector tables, codes, and initialized data */
    .isr_vector :
    {
        *(.vectors) /* be careful with --gc-sections */
    } > ROM /* make sure the vector table begins at init_svtor */

    .text :
    {
        *(.text) /* be careful with -ffunction-sections */
    } > ROM

    .rodata :
    {
        *(.rodata)
    } > ROM

    .data :
    {
        _data = .; /* put this symbol at this addr */
        *(.data) /* be careful with -fdata-sections */
        _edata = .; /* put this symbol at this addr */
    } > ROM AT> ROM /* .data should be modifiable during runtime */
    _data_rom = LOADADDR(.data); /* get LMA of this region */


    /* the uninit data, and zero-init data (.bss) should be place to RAM */
    .bss :
    {
        _bstart = .;
        *(.bss)
        *(COMMON)
        _bend = .;
    } > RAM AT> RAM
    /* it's crucial to specify the LMA when you want to change LMA region if you
     * didn't give a VALID VMA ADDR to this region. check the doc for details:
     * https://sourceware.org/binutils/docs/ld/Output-Section-LMA.html */
}

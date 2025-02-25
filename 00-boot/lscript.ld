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
    .text (ORIGIN(ROM)) :
    {
        /* make sure the vector table begins at init_svtor */
        *(.vectors) /* be careful with --gc-sections */
        *(.text) /* be careful with -ffunction-sections */
        *(.data) /* be careful with -fdata-sections */
        *(.rodata)
    }

    /* the uninit data, and zero-init data (.bss) should be place to RAM */
    .bss (ORIGIN(RAM)) :
    {
        *(.bss)
    }

    /* defined for reset handler to reset .bss section */
    __bss_start = ADDR(.bss);
    __bss_size = SIZEOF(.bss);
    __bss_end = ADDR(.bss) + __bss_size;
}

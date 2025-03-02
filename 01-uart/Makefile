CROSS_COMPILE = arm-none-eabi-

CC = gcc
CFLAGS = -g3 -O0 -Wall -mcpu=cortex-m33 -mthumb -nostartfiles --specs=nosys.specs -nostdlib -nostdinc -I../inc

LDFLAGS = -Map=output.map --script=lscript.ld

main.elf: startup.o tests.o main.o
	$(CROSS_COMPILE)$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o $@ -c $<

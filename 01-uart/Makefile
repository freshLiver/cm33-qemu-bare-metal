CROSS_COMPILE = arm-none-eabi-

CC = gcc
CFLAGS = -g3 -O0 -Wall -mcpu=cortex-m33 -mthumb -nostartfiles --specs=nosys.specs -nostdlib -nostdinc -I../inc

LDFLAGS = -Map=output.map --script=lscript.ld

UTIL_OBJS   = ../utils/uart.o
OBJS        = startup.o tests.o main.o $(UTIL_OBJS)

main.elf: $(OBJS)
	$(CROSS_COMPILE)$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o $@ -c $<

GCC_OPTIONS = -g -O1 -mcpu=cortex-m0 -mthumb
AS_OPTIONS = -g -mcpu=cortex-m0 -mthumb
LINK_OPTIONS = -g -mcpu=cortex-m0 -mthumb --specs=rdimon.specs -lc -lrdimon

HW1.elf: test.o t1.o
	arm-none-eabi-gcc $(LINK_OPTIONS) test.o t1.o -o HW1.elf

test.o: test.c
	arm-none-eabi-gcc $(GCC_OPTIONS) -c test.c -o test.o

t1.o: t1.s
	arm-none-eabi-as $(AS_OPTIONS) -c t1.s -o t1.o

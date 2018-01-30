arm-none-eabi-gcc options:
	-g -01 -mcpu=cortex-m0 -mthumb
arm-none-eabi-as options:
	-g -mcpu=cortex-m0 -mthumb
linker options:
	arm-none-eabi-gcc -g -mcpu=cortex-m0 -mthumb --specs=rdimon.specs -lc -lrdimon

test: HW1.elf
	qemu-system-arm -machine versatilepb -cpu cortex-m3 -nographic \ -monitor null -serial null \ -semihosting -kernel HW1.elf

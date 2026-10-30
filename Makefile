CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O1 -g -Wall -ffreestanding
LDFLAGS = -nostdlib -nostartfiles -T STM32F401CC.ld -Wl,--gc-sections

all: build/turtle.elf

build/turtle.elf: src/main.c src/startup.c STM32F401CC.ld
	mkdir -p build
	$(CC) $(CFLAGS) src/main.c src/startup.c $(LDFLAGS) -o $@

clean:
	rm -rf build

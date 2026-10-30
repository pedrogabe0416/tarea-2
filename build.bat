@echo off
if not exist build mkdir build
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O1 -g -Wall -ffreestanding src\main.c src\startup.c -nostdlib -nostartfiles -T STM32F401CC.ld -Wl,--gc-sections -o build\turtle.elf
if %errorlevel% neq 0 (
  echo ERROR al compilar
) else (
  echo Listo: build\turtle.elf
)
pause

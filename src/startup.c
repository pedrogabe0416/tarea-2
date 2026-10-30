/* Arranque minimo para STM32F401CC (Cortex-M4) */
#include <stdint.h>

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;

extern int main(void);
void SysTick_Handler(void);

void Reset_Handler(void);
void Default_Handler(void);

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;

    while (dst < &_edata) { *dst++ = *src++; }   /* copiar .data de flash a RAM */
    dst = &_sbss;
    while (dst < &_ebss)  { *dst++ = 0; }        /* limpiar .bss */

    main();
    while (1) { }
}

void Default_Handler(void)
{
    while (1) { }
}

__attribute__((section(".isr_vector"), used))
void (* const vector_table[])(void) = {
    (void (*)(void))(&_estack),   /*  0: pila inicial */
    Reset_Handler,                /*  1: Reset */
    Default_Handler,              /*  2: NMI */
    Default_Handler,              /*  3: HardFault */
    Default_Handler,              /*  4: MemManage */
    Default_Handler,              /*  5: BusFault */
    Default_Handler,              /*  6: UsageFault */
    0, 0, 0, 0,                   /*  7-10: reservados */
    Default_Handler,              /* 11: SVC */
    Default_Handler,              /* 12: DebugMon */
    0,                            /* 13: reservado */
    Default_Handler,              /* 14: PendSV */
    SysTick_Handler               /* 15: SysTick */
};

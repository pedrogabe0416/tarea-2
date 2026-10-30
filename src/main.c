/* Turtle Proteus - STM32F401CC (Black Pill)
 * Cableado del proyecto original:
 * PA0 -> IN1, PA1 -> IN2, PA2 -> IN3, PA3 -> IN4,
 * PA4 -> ENA, PA5 -> ENB.
 * Secuencia: adelante, atras, derecha e izquierda, 2 s cada una.
 */
#include <stdint.h>

#define REG(addr) (*(volatile uint32_t *)(addr))
#define RCC_AHB1ENR REG(0x40023830u)
#define GPIOA_MODER REG(0x40020000u)
#define GPIOA_ODR   REG(0x40020014u)
#define SYST_CSR    REG(0xE000E010u)
#define SYST_RVR    REG(0xE000E014u)
#define SYST_CVR    REG(0xE000E018u)

/* Mapeo del main.c incluido en turtle_stm32.tar */
#define IN1 (1u << 0)  /* PA0 */
#define IN2 (1u << 1)  /* PA1 */
#define IN3 (1u << 2)  /* PA2 */
#define IN4 (1u << 3)  /* PA3 */
#define ENA (1u << 4)  /* PA4 */
#define ENB (1u << 5)  /* PA5 */
#define MASCARA 0x3Fu

#define TIEMPO_MOVIMIENTO_MS 2000u
#define TIEMPO_PAUSA_MS       500u

/* SysTick se consulta por COUNTFLAG; no se usa WFI ni interrupcion. */
void SysTick_Handler(void)
{
}

static void iniciar_systick(void)
{
    SYST_CSR = 0u;
    SYST_RVR = 16000u - 1u;  /* 1 ms a 16 MHz */
    SYST_CVR = 0u;
    SYST_CSR = 5u;           /* CPU clock, ENABLE=1, TICKINT=0 */
}

static void esperar_ms(uint32_t ms)
{
    while (ms > 0u)
    {
        (void)SYST_CSR; /* limpia una posible bandera anterior */
        while ((SYST_CSR & (1u << 16)) == 0u)
        {
        }
        ms--;
    }
}

static void iniciar_gpio(void)
{
    RCC_AHB1ENR |= 1u;
    (void)RCC_AHB1ENR;
    GPIOA_MODER &= ~0x00000FFFu;
    GPIOA_MODER |=  0x00000555u; /* PA0..PA5 como salidas */
    GPIOA_ODR &= ~MASCARA;
}

static void motores(uint32_t direccion)
{
    GPIOA_ODR = (GPIOA_ODR & ~MASCARA) | ENA | ENB | direccion;
}

static void parar(void)
{
    GPIOA_ODR &= ~MASCARA;
}

static void ejecutar_movimiento(uint32_t direccion)
{
    motores(direccion);
    esperar_ms(TIEMPO_MOVIMIENTO_MS);
    parar();
    esperar_ms(TIEMPO_PAUSA_MS);
}

int main(void)
{
    iniciar_gpio();
    iniciar_systick();
    esperar_ms(500u);

    while (1)
    {
        ejecutar_movimiento(IN1 | IN3); /* adelante */
        ejecutar_movimiento(IN2 | IN4); /* atras */
        ejecutar_movimiento(IN1 | IN4); /* derecha: ruedas en sentidos opuestos */
        ejecutar_movimiento(IN2 | IN3); /* izquierda: ruedas en sentidos opuestos */
    }
}

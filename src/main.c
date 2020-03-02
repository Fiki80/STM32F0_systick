/**
 * Main program.
 */
#include <stdint.h>
#include "stm32f0xx.h"
#include "tinyprintf.h"

#define LED (4)

volatile uint32_t msTick = 0;


void SysTick_Init(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);	// interrupt every 1ms
}

void RCC_Init(void)
{
	// enable HSE
	RCC->CR |= RCC_CR_HSEON | (RCC->CR & ~RCC_CR_HSEBYP); 
	while ((RCC->CR & RCC_CR_HSERDY) == 0);
	
	// enable PLL
	RCC->CFGR |= (0x4U << 18) | (0x1U << 16);
	RCC->CR |= (0x1U << 24);	// PLLON
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);

	// set wait state and enable prefetch buffer
	FLASH->ACR |= (FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY);

	// switch clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void GPIO_Init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~(0x3U << (LED*2));
	GPIOA->MODER |= (0x1U << (LED*2));
	GPIOA->OTYPER &= ~(0x1U << LED);
	GPIOA->ODR &= ~(0x1U << LED);
}

void USART1_Init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;		// enable clock for GPIOA
	GPIOA->MODER |= (0x2U << 18);			// PA9 alt.
	GPIOA->MODER |= (0x2U << 20);			// PA10 alt.

	// set alternate function on pins PA9, PA10
	GPIOA->AFR[1] |= (0x00000110);

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	// enable clock for USART1
	USART1->BRR = SystemCoreClock/115200;	// set baudrate
	
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_UE);			// enable transmitter
	USART1->CR1 |= USART_CR1_UE;							// enable USART
}

void _putc(void *p, char c)
{
	while (!(USART1->ISR & USART_ISR_TXE));
	USART1->TDR = c;

	if (c == '\n')
	{
		while (!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = '\r';

	}
}

int main(void) {

	init_printf(NULL, _putc);

	RCC_Init();
	SysTick_Init();
	GPIO_Init();
	USART1_Init();

	while(1)
	{
	}
}




void SysTick_Handler(void)
{
	msTick++;
	if (msTick >= 500)
	{
		msTick = 0;
		GPIOA->ODR ^= (0x1U << LED);
		printf("%s%i%c", "Core clock: ", SystemCoreClock, '\n');
	}
}


/**
 * Main program.
 */
#include "stm32f0xx.h"
#include <stdint.h>

#define LED (4)

volatile uint32_t msTick = 0;

void SysTick_Init(void)
{
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
	GPIOA->MODER |= (0x2U << 4);			// PA2 alt. out push-pull

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	// enable clock for USART1
	USART1->BRR = 48000000L/115200L;		// set baudrate
	
	USART1->CR1 |= USART_CR1_TE;			// enable transmitter
	USART1->CR1 |= USART_CR1_UE;			// enable USART
}

uint8_t sendChar(uint8_t ch)
{
	while (!(USART1->ISR & USART_ISR_TXE));
	USART1->TDR = ch;
	return (ch);
}

int main(void) {

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
	}
}


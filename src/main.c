/**
 * Main program.
 */
#include "stm32f0xx.h"
#include <stdint.h>

#define LED (4)

void gpio_init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~(0x3U << (LED*2));
	GPIOA->MODER |= (0x1U << (LED*2));
	GPIOA->OTYPER &= ~(0x1U << LED);
	GPIOA->ODR |= (0x1U << LED);
}

int main(void) {
	uint8_t val = 0;
	
	while (1) {
		val += 1;
	}
}


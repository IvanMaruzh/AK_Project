#include "dht.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

void delay_init()
{
	  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	  TIM4->PSC = SystemCoreClock / 1000000 - 1;
	  TIM4->ARR = 0xFFFFFFFF;

	  TIM4->CR1 |= TIM_CR1_ARPE;
	  TIM4->EGR |= TIM_EGR_UG;

	  TIM4->CR1 |= TIM_CR1_CEN;
}

void micro_delay(uint32_t microseconds)
{
	uint32_t clk_cycle_start = TIM4->CNT;
	while ((TIM4->CNT - clk_cycle_start) < microseconds);
}

void DHT11_PortInit()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER1_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_0;
}


void DHT11_Start()
{
	delay_init();
	GPIOA->MODER |= GPIO_MODER_MODER1_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1_0;

	GPIOA->BSRR |= GPIO_BSRR_BR_1;
	micro_delay(20000);   // wait for 20ms
	GPIOA->BSRR |= GPIO_BSRR_BS_1;
	micro_delay(30);

	GPIOA->MODER &= ~GPIO_MODER_MODER1_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1_0;
}

uint8_t DHT11_Response()
{
	uint8_t Response = 0;
	micro_delay(40);
	if ((GPIOA->IDR & GPIO_PIN_1) == (uint32_t)GPIO_PIN_RESET)
	{
		micro_delay(80);

		if ((GPIOA->IDR & GPIO_PIN_1) != (uint32_t)GPIO_PIN_RESET)
			Response = 1;
	}

	while ((GPIOA->IDR & GPIO_PIN_1) != (uint32_t)GPIO_PIN_RESET);   // wait for the pin to go low

	return Response;
}

uint8_t DHT11_Read()
{
	uint8_t a, b;

	for (a = 0; a < 8; a++)
	{
		while ((GPIOA->IDR & GPIO_PIN_1) == (uint32_t)GPIO_PIN_RESET);
		micro_delay(40);   // wait for 40 us

		if ((GPIOA->IDR & GPIO_PIN_1) == (uint32_t)GPIO_PIN_RESET)   // if the pin is low
			b &= ~(1<<(7-a));
		else
			b |= (1<<(7-a));

		while ((GPIOA->IDR & GPIO_PIN_1) != (uint32_t)GPIO_PIN_RESET);
	}
	return b;
}

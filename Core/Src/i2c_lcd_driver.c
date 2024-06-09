#include "i2c_lcd_driver.h"


void i2c_port_init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	GPIOB->MODER |= GPIO_MODER_MODE8_1;
	GPIOB->MODER |= GPIO_MODER_MODE9_1;

	GPIOB->AFR[1] |= GPIO_AF4_I2C1<<0 | GPIO_AF4_I2C1<<4;

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	I2C1->CR2 |= 0b110010; //frequency
	I2C1->TRISE |= 0b110011;
	I2C1->CCR |= 250; //time

	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR1 |= I2C_CR1_ACK;
}

void i2c_write_bytes(uint16_t addr, uint8_t *buf, uint16_t bytes_count)
{
  uint16_t i;

  I2C1->CR1 |= I2C_CR1_ACK;
  I2C1->CR1 |= I2C_CR1_START;
  while (!READ_BIT(I2C1->SR1, I2C_SR1_SB));
  (void) I2C1->SR1; //check state

  I2C1->DR = addr | 0x00;
  while (!READ_BIT(I2C1->SR1, I2C_SR1_ADDR));
  (void) I2C1->SR1;
  (void) I2C1->SR2;

  for (i = 0; i < bytes_count; i++)
  {
	  I2C1->DR &= ~I2C_DR_DR;
	  I2C1->DR |= buf[i];
	  while (!READ_BIT(I2C1->SR1, I2C_SR1_TXE));
  }
  I2C1->CR1 |= I2C_CR1_STOP;
}

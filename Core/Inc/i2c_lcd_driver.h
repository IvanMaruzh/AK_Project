#ifndef INC_I2C_LCD_DRIVER_H_
#define INC_I2C_LCD_DRIVER_H_

#include "stm32f4xx.h"
#include <stdio.h>
#define SLAVE_ADDRESS_LCD 0x4E

void i2c_port_init(void);
void i2c_transmit_data8(uint8_t Data);
void i2c_write_bytes(uint16_t addr, uint8_t *buf, uint16_t bytes_count);

#endif /* INC_I2C_LCD_DRIVER_H_ */

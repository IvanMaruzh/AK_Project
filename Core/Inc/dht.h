#ifndef DHT_H_
#define DHT_H_

#include "stdint.h"
#include <stm32f4xx.h>

void delay_init(void);
void micro_delay(uint32_t);
void DHT11_PortInit(void);
void DHT11_Start(void);
uint8_t DHT11_Response(void);
uint8_t DHT11_Read(void);

#endif /* DHT_H_ */

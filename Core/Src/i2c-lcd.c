#include "i2c-lcd.h"


void lcd_send_cmd(char cmd)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  // 4 upper bits
	data_t[1] = data_u|0x08;  // enable
	data_t[2] = data_l|0x0C;  // 4 lower bits
	data_t[3] = data_l|0x08;  // enable
	i2c_write_bytes(SLAVE_ADDRESS_LCD, (uint8_t *) data_t, 4);
}

void lcd_send_data(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  // 4 upper bits
	data_t[1] = data_u|0x09;  // enable
	data_t[2] = data_l|0x0D;  // 4 lower bits
	data_t[3] = data_l|0x09;  // enable
	i2c_write_bytes(SLAVE_ADDRESS_LCD, (uint8_t *) data_t, 4);
}

void lcd_clear()
{
	lcd_send_cmd(0x01);
	HAL_Delay(10);
}

void lcd_put_cur(int row, int col)
{
	switch (row)
    {
		case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd(col);
}

void lcd_init()
{
	HAL_Delay(50); // delay for 50ms
	lcd_send_cmd(0x30);
	HAL_Delay(5); // delay for 5ms
	lcd_send_cmd(0x30);
	HAL_Delay(1); // delay for 1ms
	lcd_send_cmd(0x30);
	HAL_Delay(10); // delay for 10ms
	lcd_send_cmd(0x20);
	HAL_Delay(10);

	// Start display
	lcd_send_cmd(0x28); // Function set
	HAL_Delay(1);
	lcd_send_cmd(0x08); // Display on/off control
	HAL_Delay(1);
	lcd_send_cmd(0x01); // Clear display
	HAL_Delay(1);
	lcd_send_cmd(0x06); // Entry mode set
	HAL_Delay(1);
	lcd_send_cmd(0x0C); // Display on/off control
}

void lcd_send_string(char *str)
{
	while (*str) lcd_send_data(*str++);
}

//-------------------------------------------------------------------------------------------
// Title     : I2C Master-Slave Communication (STM32L476)
// Author    : Umar Wahid
// Target    : STM32L476 Series Microcontroller
// Purpose   : Read accelerometer data (X, Y, Z axes) from MPU6050 over I2C3 interface
// Version   : 1.18.1 (STM32CubeIDE)
// Date      : June 21, 2025
// Notes     :
//             - I2C Master: STM32L476 using I2C3 peripheral
//             - I2C3 Pin Mapping:
//                   * PC0 - I2C3_SCL (Alternate Function AF4)
//                   * PC1 - I2C3_SDA (Alternate Function AF4)
//             - UART2 TX (for printf):
//                   * PA2 - USART2_TX (Alternate Function AF7)
//             - I2C Timing Register configured for Standard Mode (100kHz) @ 4 MHz SYSCLK
//             - Pull-up resistors enabled on I2C lines via GPIO configuration
//             - MPU6050 default I2C address: 0x68
//             - System Clock Frequency: 4 MHz (internal)
//             - UART Baudrate: 115200 (8N1 format)
//             - Output: Acceleration data printed to serial terminal over UART2
//-------------------------------------------------------------------------------------------


#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx.h"
#include "uart.h"
#include "i2c.h"
#include "mpu6050.h"
#include "systick.h"




int main(void)
{
    uart2_tx_init();
    i2c3_master_config();
    i2c3_write_byte(0x68, 0x6B, 0x00);  // Wake up MPU6050
    for (volatile int i = 0; i < 10000; ++i);

    uint8_t pwr = i2c3_read_byte(0x68, 0x6B);
    printf("PWR_MGMT_1 = 0x%X\r\n", pwr);




    while (1)
    {

    	//x axis
    	uint8_t axh = i2c3_read_byte(0x68, 0x3B);
    	uint8_t axl = i2c3_read_byte(0x68, 0x3c);
    	int16_t ax = ((int16_t)axh << 8) | axl;
    	int32_t ax_g_scaled = (ax * 100) / 16384;


    	//y axis
    	uint8_t ayh = i2c3_read_byte(0x68, 0x3D);
    	uint8_t ayl = i2c3_read_byte(0x68, 0x3E);
    	int16_t ay = ((int16_t)ayh << 8) | ayl;
    	int32_t ay_g_scaled = (ay * 100) / 16384;

    	//z axis
    	uint8_t azh = i2c3_read_byte(0x68, 0x3F);
    	uint8_t azl = i2c3_read_byte(0x68, 0x40);
    	int16_t az = ((int16_t)azh << 8) | azl;
    	int32_t az_g_scaled = (az * 100) / 16384;

    	printf("AX (g) = %ld.%02ld\r\n", ax_g_scaled / 100, ax_g_scaled % 100);
    	systickDelayMs(1000);
    	printf("AY (g) = %ld.%02ld\r\n", ay_g_scaled / 100, ay_g_scaled % 100);
    	systickDelayMs(1000);
    	printf("AZ(g) = %ld.%02ld\r\n", az_g_scaled / 100, az_g_scaled % 100);
    	systickDelayMs(1000);

    }
}








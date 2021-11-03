#include "device_drivers/l3gd20.h"
#include <stdio.h>
#include <string.h>


static uint8_t spiTxBuf[2];
static uint8_t spiRxBuf[7];

void l3gd20_init(SPI_HandleTypeDef *hspi) {
	uint8_t data[2];

	data[0] = 0x20;
	data[1] = 0xFF;

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(20);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(20);
	data[0] = 0x20;
	data[1] = 0xbf;
	HAL_SPI_Transmit(hspi, data, 2, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(20);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(20);
	data[0] = 0x21;
	data[1] = 0x00;
	HAL_SPI_Transmit(hspi, data, 2, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(20);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(20);
	data[0] = 0x22;
	data[1] = 0x00;
	HAL_SPI_Transmit(hspi, data, 2, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(20);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(20);
	data[0] = 0x23;
	data[1] = 0x30;
	HAL_SPI_Transmit(hspi, data, 2, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(20);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(20);
	data[0] = 0x24;
	data[1] = 0x10;
	HAL_SPI_Transmit(hspi, data, 2, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_Delay(20);
}

struct GyroAxisData l3gd20_get_axis_data(SPI_HandleTypeDef *hspi) {
	volatile int16_t Raw_x = 0;
	volatile int16_t Raw_y = 0;
	volatile int16_t Raw_z = 0;

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	spiTxBuf[0] = 0x28 | 0x80;
	HAL_SPI_Transmit(hspi, spiTxBuf, 1, 50);
	HAL_SPI_Receive(hspi, &spiRxBuf[1], 1, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	spiTxBuf[0] = 0x29 | 0x80;
	HAL_SPI_Transmit(hspi, spiTxBuf, 1, 50);
	HAL_SPI_Receive(hspi, &spiRxBuf[2], 1, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	spiTxBuf[0] = 0x2a | 0x80;
	HAL_SPI_Transmit(hspi, spiTxBuf, 1, 50);
	HAL_SPI_Receive(hspi, &spiRxBuf[3], 1, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	spiTxBuf[0] = 0x2b | 0x80;
	HAL_SPI_Transmit(hspi, spiTxBuf, 1, 50);
	HAL_SPI_Receive(hspi, &spiRxBuf[4], 1, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	spiTxBuf[0] = 0x2c | 0x80;
	HAL_SPI_Transmit(hspi, spiTxBuf, 1, 50);
	HAL_SPI_Receive(hspi, &spiRxBuf[5], 1, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
	spiTxBuf[0] = 0x2d | 0x80;
	HAL_SPI_Transmit(hspi, spiTxBuf, 1, 50);
	HAL_SPI_Receive(hspi, &spiRxBuf[6], 1, 50);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	Raw_x = (spiRxBuf[2] << 8) | spiRxBuf[1];
	Raw_y = (spiRxBuf[4] << 8) | spiRxBuf[3];
	Raw_z = (spiRxBuf[6] << 8) | spiRxBuf[5];

	struct GyroAxisData axisData;
	axisData.x = (0.07 * Raw_x) + 1;
	axisData.y = (0.07 * Raw_y) - 0.2;
	axisData.z = (0.07 * Raw_z) + 1;

	// 245 -> 0.00875
	// 500 -> 0.0175
	// 2000 -> 0.07

	return axisData;
}


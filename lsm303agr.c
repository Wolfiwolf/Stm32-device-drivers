#include "device_drivers/lsm303agr.h"
#include <stdio.h>

void lsm303agr_init(I2C_HandleTypeDef *hi2c) {
	uint8_t address = 0x19 << 1;
	uint8_t buffer[3];

	buffer[0] = 0x21;
	buffer[1] = 0x00;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 2, HAL_MAX_DELAY);

	buffer[0] = 0x22;
	buffer[1] = 0x00;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 2, HAL_MAX_DELAY);

	buffer[0] = 0x23;
	buffer[1] = 0x81;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 2, HAL_MAX_DELAY);

	buffer[0] = 0x20;
	buffer[1] = 0x5F;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 2, HAL_MAX_DELAY);
	HAL_Delay(90);
}

struct AccelerometerAxisData lsm303agr_get_axis_data(I2C_HandleTypeDef *hi2c) {
	uint8_t address = 0x19 << 1;
	uint8_t buffer[3];

	buffer[0] = 0x28;
	buffer[1] = 0x1;
	buffer[2] = 0x1;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 1, 3000);
	HAL_I2C_Master_Receive(hi2c, address, &buffer[1], 1, 3000);

	buffer[0] = 0x29;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 1, 3000);
	HAL_I2C_Master_Receive(hi2c, address, &buffer[2], 1, 3000);

	int16_t xAxis = buffer[2];
	xAxis <<= 8;
	xAxis |= buffer[1];
	xAxis >>= 6;

	float x = xAxis;
	x /= 252.0f;

	buffer[0] = 0x2A;
	buffer[1] = 0x1;
	buffer[2] = 0x1;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 1, 3000);
	HAL_I2C_Master_Receive(hi2c, address, &buffer[1], 1, 3000);

	buffer[0] = 0x2B;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 1, 3000);
	HAL_I2C_Master_Receive(hi2c, address, &buffer[2], 1, 3000);

	int16_t yAxis = buffer[2];
	yAxis <<= 8;
	yAxis |= buffer[1];
	yAxis >>= 6;

	float y = yAxis;
	y /= 252.0f;

	buffer[0] = 0x2C;
	buffer[1] = 0x1;
	buffer[2] = 0x1;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 1, 3000);
	HAL_I2C_Master_Receive(hi2c, address, &buffer[1], 1, 3000);

	buffer[0] = 0x2D;
	HAL_I2C_Master_Transmit(hi2c, address, buffer, 1, 3000);
	HAL_I2C_Master_Receive(hi2c, address, &buffer[2], 1, 3000);

	int16_t zAxis = buffer[2];
	zAxis <<= 8;
	zAxis |= buffer[1];
	zAxis >>= 6;

	float z = zAxis;
	z /= 252.0f;

	struct AccelerometerAxisData axisData;
	axisData.x = x;
	axisData.y = y + 0.03f;
	axisData.z = z;

	return axisData;
}

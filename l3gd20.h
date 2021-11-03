#ifndef L3GD20_H_FILE
#define L3GD20_H_FILE

#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"

struct GyroAxisData {
	float x, y, z;
};

void l3gd20_init(SPI_HandleTypeDef *hspi);

struct GyroAxisData l3gd20_get_axis_data(SPI_HandleTypeDef *hspi);

#endif

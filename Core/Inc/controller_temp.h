#ifndef __CTRL_TEMP_H__
#define __CTRL_TEMP_H__

#include "sht2x_for_stm32_hal.h"

typedef struct
{
    float temp;
    float hum;
} Ctrl_Temp;


void
ctrl_temp_init(Ctrl_Temp *temp, I2C_HandleTypeDef *i2c);

float
ctrl_temp_temp(Ctrl_Temp *temp);

float
ctrl_temp_hum(Ctrl_Temp *temp);

#endif /* __CTRL_TEMP_H__ */

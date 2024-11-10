#include "controller_temp.h"

void
ctrl_temp_init(Ctrl_Temp *temp, I2C_HandleTypeDef *i2c)
{
    SHT2x_Init(i2c);
    SHT2x_SetResolution(RES_14_12);
}

float
ctrl_temp_temp(Ctrl_Temp *temp)
{
    return SHT2x_GetTemperature(1);
}

float
ctrl_temp_hum(Ctrl_Temp *temp)
{
    return SHT2x_GetRelativeHumidity(1);
}

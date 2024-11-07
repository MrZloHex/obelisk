/**
  ******************************************************************************
  * @file    controller.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "lcd_i2c.h"
#include "stdint.h"


typedef struct Controller_S
{
	UART_HandleTypeDef *vcon;
	TIM_HandleTypeDef *vcon_tim;
	RTC_HandleTypeDef *rtc;
	LCD_I2C 	lcd;
} Controller;

void
controller_initialize
(
	Controller *controller,
	UART_HandleTypeDef *vcon,
	TIM_HandleTypeDef *vcon_tim,
	RTC_HandleTypeDef *rtc,
	I2C_HandleTypeDef *i2c
);

void
controller_update(Controller *controller);

#endif /* __CONTROLLER_H__ */

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

#include "controller_datetime.h"
#include "controller_lcd.h"
#include "controller_temp.h"
#include "controller_uart.h"

typedef struct Controller_S
{
	Ctrl_LCD      lcd;
    Ctrl_DateTime datetime;
    Ctrl_Temp     temp;
    Ctrl_UART     uart;
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

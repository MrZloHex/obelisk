#include "controller.h"


void
controller_initialize
(
	Controller *controller,
	UART_HandleTypeDef *vcon,
	TIM_HandleTypeDef *vcon_tim,
	RTC_HandleTypeDef *rtc,
	I2C_HandleTypeDef *i2c
)
{
    ctrl_lcd_init(&(controller->lcd),   i2c);
    ctrl_datetime_init(&(controller->datetime), rtc);
    // ctrl_temp_init(&(controller->temp), i2c);
    // ctrl_uart_init(&(controller->uart), vcon, vcon_tim, '\n');
    ctrl_lcd_update_time(&(controller->lcd), controller->datetime.time);
    ctrl_lcd_update_date(&(controller->lcd), controller->datetime.date);
}

void
controller_update(Controller *ctrl)
{
    ctrl_datetime_update(&(ctrl->datetime));
    if (ctrl->datetime.new_date)
    {
        ctrl_lcd_update_date(&(ctrl->lcd), ctrl->datetime.date);
    }
    if (ctrl->datetime.new_time)
    {
        ctrl_lcd_update_time(&(ctrl->lcd), ctrl->datetime.time);
    }

    // ctrl_lcd_update_temp(&(ctrl->lcd), ctrl_temp_temp(&(ctrl->temp)));
}

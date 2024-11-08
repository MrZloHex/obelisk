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
    *controller = (Controller)
    {
        .vcon = vcon,
        .vcon_tim = vcon_tim,
    };

    ctrl_lcd_init(&(controller->lcd),   i2c);
    ctrl_datetime_init(&(controller->datetime), rtc);
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
}

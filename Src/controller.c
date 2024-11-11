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
    ctrl_server_init(&(controller->server), vcon, vcon_tim);
    ctrl_lcd_init(&(controller->lcd),   i2c);
    ctrl_datetime_init(&(controller->datetime), rtc);

    ctrl_lcd_update_time(&(controller->lcd), controller->datetime.time);
    ctrl_lcd_update_date(&(controller->lcd), controller->datetime.date);

    ctrl_temp_init(&(controller->temp), i2c);
    timer_set(&(controller->temp_tim), 500, true);
    timer_start(&(controller->temp_tim));
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
    

    if (timer_timeout(&(ctrl->temp_tim)))
    {
        ctrl_lcd_update_temp(&(ctrl->lcd), ctrl_temp_temp(&(ctrl->temp)));
        ctrl_lcd_update_hum(&(ctrl->lcd), ctrl_temp_hum(&(ctrl->temp)));
    }

    ctrl_server_update(&(ctrl->server), ctrl->state);
    if (ctrl_server_recv(&(ctrl->server), &(ctrl->state)) == CTRL_STATE_UPD)
    {
       ctrl_lcd_update_state(&(ctrl->lcd), ctrl->state);
    }
}

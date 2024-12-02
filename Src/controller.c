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
    controller->state.rtc = rtc;
    ctrl_server_init(&(controller->server), vcon, vcon_tim);
    ctrl_lcd_init(&(controller->lcd),   i2c);
    ctrl_datetime_init(&(controller->datetime), rtc);

    ctrl_lcd_update_time(&(controller->lcd), controller->datetime.time);
    ctrl_lcd_update_date(&(controller->lcd), controller->datetime.date);

    ctrl_temp_init(&(controller->temp), i2c);
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
    
    ctrl_server_update(&(ctrl->server), ctrl->state);
    Ctrl_UPD msg;
    if (ctrl_server_recv(&(ctrl->server), &(ctrl->state), &msg) == CTRL_STATE_UPD)
    {
        if (msg == UPD_STATE)
        { ctrl_lcd_update_state(&(ctrl->lcd), ctrl->state); }
        if (msg == UPD_WTHR)
        { ctrl_lcd_update_wthr(&(ctrl->lcd), ctrl->state.weather); }
    }
}

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
        .rtc = rtc
    };

    lcd_i2c_init(&(controller->lcd), i2c, 0x4E, LCD_20x4, 10);
}

void
controller_update(Controller *controller)
{

}

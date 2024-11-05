#ifndef __CONTROLLER_UART_H__
#define __CONTROLLER_UART_H__

#include "controller_types.h"

#include "stdbool.h"
#include "serial.h"

#define BUF_LEN 512
#define CTRL_UART_BUFFER(__INDEX__)	(uint8_t*)&ctrl_uart->buffer.content[__INDEX__]
#define CTRL_UART_CURRENT_INDEX		ctrl_uart->buffer.index
#define CTRL_UART_CURRENT_CHARACTER	ctrl_uart->buffer.content[CTRL_UART_CURRENT_INDEX]
#define CTRL_UART_NEWLINE(__TERM__)	(CTRL_UART_CURRENT_CHARACTER == __TERM__)
#define CTRL_UART_BUFFER_FULL		(CTRL_UART_CURRENT_INDEX > BUF_LEN-1)
#define CTRL_UART_OVERFLOW			(CTRL_UART_BUFFER_FULL || ctrl_uart->buffer.state == UART_STATE_OVERFLOW)
#define CTRL_UART_RECEIVING			(ctrl_uart->buffer.state == UART_STATE_RECEIVING)

typedef enum UART_Buffer_State_E
{
	UART_STATE_UNDEFINED	= 0x0U,
	UART_STATE_IDLE			= 0x1U,
	UART_STATE_RECEIVING	= 0x2U,
	UART_STATE_RECEIVED 	= 0x3U,
	UART_STATE_PROCESSING	= 0x4U,
	UART_STATE_OVERFLOW		= 0x5U,
	UART_STATE_UNKNOWN		= 0xFU
} UART_Buffer_State;

typedef struct UART_Buffer_S
{
	char content[BUF_LEN];
	size_t index;
	UART_Buffer_State state;
} UART_Buffer;

typedef struct Controller_UART_S
{
	UART_HandleTypeDef	*huart;
	TIM_HandleTypeDef	*tim;
	UART_Buffer			buffer;
	HAL_StatusTypeDef	status;
	char 				term;
} Controller_UART;


Controller_Protocol_Result
controller_uart_init
(
	Controller_UART *ctrl_uart,
	UART_HandleTypeDef *huart,
	TIM_HandleTypeDef *htim,
	char term
);

Controller_Protocol_Result
controller_uart_transmit_msg
(
	Controller_UART *ctrl_uart,
	const char *msg
);

Controller_Protocol_Result
controller_uart_recieve_msg
(
	Controller_UART *ctrl_uart,
	char *buf
);

void
controller_uart_usart_callback(Controller_UART *ctrl_uart);

void
controller_uart_timer_callback(Controller_UART *ctrl_uart);

#endif /* __CONTROLLER_UART_H__ */
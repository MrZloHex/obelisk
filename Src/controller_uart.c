#include "controller_uart.h"
#include "string.h"
#include "tim.h"


/* reset character timeout timer  */
static void
_controller_uart_reset_timer(Controller_UART *ctrl_uart)
{
	__HAL_TIM_SET_COUNTER(ctrl_uart->tim, 0);
}

static void
_controller_uart_receive(Controller_UART *ctrl_uart)
{	
	ctrl_uart->status = HAL_UART_Receive_IT(ctrl_uart->huart, CTRL_UART_BUFFER(CTRL_UART_CURRENT_INDEX), 1);
}


/* continue receiving at the end of the buffer */
static void
_controller_uart_rx_dump(Controller_UART *ctrl_uart)
{
	_controller_uart_receive(ctrl_uart);
}


static void
_controller_uart_rx_abort(Controller_UART *ctrl_uart)
{
	ctrl_uart->status = HAL_UART_Abort_IT(ctrl_uart->huart);
}


static void
_controller_uart_rx_init(Controller_UART *ctrl_uart)
{
	ctrl_uart->buffer.index = 0;
	ctrl_uart->buffer.state = UART_STATE_RECEIVING;
	memset(ctrl_uart->buffer.content, 0, BUF_LEN);
	_controller_uart_receive(ctrl_uart);
}


static void
_controller_uart_rx_next(Controller_UART *ctrl_uart)
{
	ctrl_uart->buffer.state = UART_STATE_RECEIVING;
	ctrl_uart->buffer.index++;
	_controller_uart_receive(ctrl_uart);
}


static void
_controller_uart_handle_overflow(Controller_UART *ctrl_uart)
{
	ctrl_uart->buffer.state = UART_STATE_OVERFLOW;
	_controller_uart_rx_dump(ctrl_uart);
}


static void
_controller_uart_handle_newline(Controller_UART *ctrl_uart)
{
	if (CTRL_UART_CURRENT_INDEX > 1)
	{
		ctrl_uart->buffer.state = UART_STATE_RECEIVED;
		ctrl_uart->buffer.content[CTRL_UART_CURRENT_INDEX] = '\0';
	}
	else 
	{
		_controller_uart_rx_init(ctrl_uart);
	}
}


Controller_Protocol_Result
controller_uart_init
(
	Controller_UART *ctrl_uart,
	UART_HandleTypeDef *huart,
	TIM_HandleTypeDef *htim,
	char term
)
{
    ctrl_uart->huart = huart;
	ctrl_uart->tim = htim;
	ctrl_uart->term = term;

  	__HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
	HAL_StatusTypeDef res = HAL_TIM_Base_Start_IT(htim);
	
	_controller_uart_rx_init(ctrl_uart);
	return (Controller_Protocol_Result)res;
}


Controller_Protocol_Result
controller_uart_transmit_msg(Controller_UART *ctrl_uart, const char *msg)
{
	return (Controller_Protocol_Result)uprintf(ctrl_uart->huart, 100, BUF_LEN+1, "%s\n", msg);
}

/**
 * @brief Returns RECEIVED if message is received, WAITING if message is not received yet
 * 
 * @param ctrl_uart 
 * @param buf 
 * @return Controller_Protocol_Result 
 */
Controller_Protocol_Result
controller_uart_recieve_msg(Controller_UART *ctrl_uart, char *buf)
{	
	if (ctrl_uart->status != HAL_OK)
	{
		return (Controller_Protocol_Result)ctrl_uart->status;
	}
	 
	if (ctrl_uart->buffer.state == UART_STATE_RECEIVED)
	{
		strcpy(buf, ctrl_uart->buffer.content);
		_controller_uart_rx_init(ctrl_uart);
		return CTRL_PTCL_RECEIVED;
	}
	else return CTRL_PTCL_WAITING;
}


void
controller_uart_usart_callback(Controller_UART *ctrl_uart)
{	
	_controller_uart_reset_timer(ctrl_uart);

	if (CTRL_UART_OVERFLOW && CTRL_UART_NEWLINE(ctrl_uart->term))	_controller_uart_rx_init(ctrl_uart);
	else if (CTRL_UART_BUFFER_FULL)					_controller_uart_handle_overflow(ctrl_uart);
	else if (CTRL_UART_NEWLINE(ctrl_uart->term))	_controller_uart_handle_newline(ctrl_uart);
	else if (CTRL_UART_RECEIVING)					_controller_uart_rx_next(ctrl_uart);
}


void
controller_uart_timer_callback(Controller_UART *ctrl_uart)
{	
	if (CTRL_UART_RECEIVING && CTRL_UART_CURRENT_INDEX > 0)
	{
		_controller_uart_rx_abort(ctrl_uart);
		_controller_uart_rx_init(ctrl_uart);
		_controller_uart_reset_timer(ctrl_uart);
	}
}

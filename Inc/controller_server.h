#ifndef __CTRL_SERVER_H__
#define __CTRL_SERVER_H__

#include "controller_uart.h"
#include "controller_state.h"
#include "timer.h"

#define SERVER_TIMEOUT 5000

typedef enum
{
    CTRL_OK,
    CTRL_BUSY,
    CTRL_ERR,
    CTRL_TIMEOUT,
    CTRL_STATE_UPD,
    CTRL_SENT
} Ctrl_Server_Result;

typedef enum
{
    MSG_BOOT,
    MSG_TIME
} Ctrl_Server_MSG;

typedef struct
{
    Ctrl_UART   uart;
    char        proc_buf[BUF_LEN];

    Timer           resp_tim;
    Ctrl_Server_MSG curr_msg;
} Ctrl_Server;

void
ctrl_server_init
(
    Ctrl_Server *srv,
	UART_HandleTypeDef *uart,
	TIM_HandleTypeDef *tim
);

Ctrl_Server_Result
ctrl_server_update(Ctrl_Server *srv, Ctrl_State state);

Ctrl_Server_Result
ctrl_server_recv(Ctrl_Server *srv, Ctrl_State *state);

#endif /* __CTRL_SERVER_H__ */

#include "controller_server.h"
#include "mjson.h"
#include "string.h"

void
ctrl_server_init
(
    Ctrl_Server *srv,
	UART_HandleTypeDef *uart,
	TIM_HandleTypeDef *tim
)
{
    ctrl_uart_init(&(srv->uart), uart, tim, '\n');
    timer_set(&(srv->resp_tim), SERVER_TIMEOUT, false);
}

const static char *srv_msgs[] =
{ "boot", "time" };

static void
_server_send(Ctrl_Server *srv, Ctrl_Server_MSG msg)
{
    char *req = mjson_aprintf("{%Q:1,%Q:%Q}", "id", "method", srv_msgs[msg]);
    ctrl_uart_transmit_msg(&(srv->uart), req);
    srv->curr_msg = msg;
    free(req);
}

Ctrl_Server_Result
ctrl_server_update(Ctrl_Server *srv, Ctrl_State state)
{
    if (srv->resp_tim.start)
    { return CTRL_BUSY; }

    if (!state.connected)
    {
        _server_send(srv, MSG_BOOT);
        timer_start(&(srv->resp_tim));
        return CTRL_SENT;
    }

    if (!state.synced)
    {
        _server_send(srv, MSG_TIME);
        timer_start(&(srv->resp_tim));
        return CTRL_SENT;
    }

    return CTRL_ERR;
}


static Ctrl_Server_Result
ctrl_server_proc(Ctrl_Server *srv, Ctrl_State *state)
{
    char result[64] = { 0 };
    int n = mjson_get_string(srv->proc_buf, strlen(srv->proc_buf), "$.result", result, sizeof(result));
    uprintf(&huart1, 10, 20, "RES %s\n", result);

    if (srv->curr_msg == MSG_BOOT && strcmp(result, "ok") == 0)
    {
        timer_stop(&(srv->resp_tim));
        state->connected = true;
        return CTRL_STATE_UPD;
    }

    if (srv->curr_msg == MSG_TIME)
    {
        timer_stop(&(srv->resp_tim));
        state->synced = true;
        return CTRL_STATE_UPD;
    }

    return CTRL_ERR;
}

Ctrl_Server_Result
ctrl_server_recv(Ctrl_Server *srv, Ctrl_State *state)
{
    if (!srv->resp_tim.start)
    { return CTRL_OK; }

    if (timer_timeout(&(srv->resp_tim)))
    { return CTRL_TIMEOUT; }

    Controller_Protocol_Result res = ctrl_uart_recieve_msg(&(srv->uart), srv->proc_buf);
    if (res == CTRL_PTCL_RECEIVED)
	{
        res = ctrl_server_proc(srv, state);
        return res;
	}

    return CTRL_ERR;
}

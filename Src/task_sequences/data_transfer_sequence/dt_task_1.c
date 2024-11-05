#include "task_sequences/data_transfer_sequence/dt_task_1.h"
#include "task_sequences/data_transfer_sequence/dt_rapi_cmd.h"

#include "mjson.h"
#include "controller_ocpp.h"
#include "controller_temperature.h"
#include "stdio.h"
#include "serial.h"
#include "util.h"

void
_dt_err_resp(Controller *ctrl, OCPP_MessageID t_id)
{ 
	OCPPDataTransferStatus st = DTS_Rejected;
	char buf[] = "";
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, buf, NULL);
	_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, t_id);
}

Task_Result
dt_task_1(Controller *ctrl, OCPP_MessageID _id)
{
    // LOGGER_LOG(&(ctrl->logger), LT_TRACE, "Task DT 1");

	Task_Result res =
	{
		.type = TRES_NEXT,
		.task =
		{
			.type = WRAP_FINISHED,
			.task = 
			{
				.func = NULL
			}
		}
	};

	char data[MAX_DATA_TRANSFER_DATA_LEN];
	int res_id = mjson_get_string(ctrl->ocpp.message.data.call.payload, strlen(ctrl->ocpp.message.data.call.payload), P_DATA, data, MAX_DATA_TRANSFER_DATA_LEN);
	if (res_id == -1)
	{
		_dt_err_resp(ctrl, _id);
		return res;
	}

	uprintf(DBUG_UART, 1000, 50, "%s\n", data);

	OCPPDataTransferStatus st = DTS_Accepted;
	if (strcmp(data, "get_alt_measurements") == 0)
	{
		char buf[32];
		sprintf(buf, "temp:%f, hum:%f", _controller_temp_get_temp(), _controller_temp_get_hum());
    	_controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, buf, NULL);
		_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);
	}
	else if (strcmp(data, "enable_debug") == 0)
	{
		ctrl->debug = true;
    	_controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, NULL, NULL);
		_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);
	}
	else if (strcmp(data, "disable_debug") == 0)
	{
		ctrl->debug = false;
    	_controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, NULL, NULL);
		_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);
	}
	// {"data": "change_config@ports_number=2" }
	else if (starts_with(data, "change_config"))
	{
		char *config = get_after(data, '@');
		if (!config) { _dt_err_resp(ctrl, _id); }

		if (starts_with(config, "ports_number"))
		{
			int ports = get_after_num(config, '=');
			if (ports < 1 || ports > 2) { _dt_err_resp(ctrl, _id); }

			ctrl->memory.ports_number = (uint8_t)ports;
			_controller_memory_store(&(ctrl->memory));

    		_controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, NULL, NULL);
			_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);
		}
		else
		{
			_dt_err_resp(ctrl, _id);
		}
		free(config);
	}
	else if (starts_with(data, "send_rapi_cmd"))
	{
		char *cmd = get_after(data, '=');
		if (!cmd) { _dt_err_resp(ctrl, _id); }

		strcpy(ctrl->rapi.transmitter_buffer, cmd);
		_rapi_send_req(&(ctrl->rapi));

		res.task.type = WRAP_IN_PROGRESS;
		res.task.task.func = dt_rapi_cmd;
		res.task.task.func_timeout = dt_rapi_cmd_to;
		res.task.task.usart = RAPI_USART;
		res.task.task.genesis_time = HAL_GetTick();
		res.task.task.type = TASK_PROCESS;

		free(cmd);
	}
	// else if (strcmp(data, "get_error") == 0)
	// {
	// 	char buf[10];
	// 	sprintf(buf, "%u %u", ctrl->memory.fatal_err.type, ctrl->memory.fatal_err.errors);
    // 	_controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, buf, NULL);
	// 	_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);
	// }
	else
	{
		_dt_err_resp(ctrl, _id);
	}


	return res;
}

void
__get_config(const char *data)
{

}


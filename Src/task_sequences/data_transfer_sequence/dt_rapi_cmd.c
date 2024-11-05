#include "task_sequences/data_transfer_sequence/dt_rapi_cmd.h"


Task_Result
dt_rapi_cmd(Controller *ctrl, OCPP_MessageID _id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "DT_RAPI_CMD\r");
#endif
	OCPPDataTransferStatus st = DTS_Accepted;
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, ctrl->rapi.processive_buffer, NULL);
	_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task =
            {
                .type = TASK_PROCESS,
                .usart = OCPP_USART,
                .id = ctrl->ocpp.id_msg -1,
                .func = dt_rapi_cmd_fin,
            }
        }
    };

    return res;
}

Task_Result
dt_rapi_cmd_to(Controller *ctrl, OCPP_MessageID _id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "DT_RAPI_CMD\r");
#endif
	OCPPDataTransferStatus st = DTS_Rejected;
    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_DATA_TRANSFER, &st, NULL, NULL);
	_controller_ocpp_send_resp(&(ctrl->ocpp), CALLRESULT, _id);

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task =
            {
                .type = TASK_PROCESS,
                .usart = OCPP_USART,
                .id = ctrl->ocpp.id_msg -1,
                .func = dt_rapi_cmd_fin,
            }
        }
    };

    return res;
}

Task_Result
dt_rapi_cmd_fin(Controller *ctrl, OCPP_MessageID _id)
{
#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "GS_3\r");
#endif

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

    return res;
}


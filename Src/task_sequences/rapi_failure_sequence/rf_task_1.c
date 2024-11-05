#include "task_sequences/rapi_failure_sequence/rf_task_1.h"
#include "task_sequences/rapi_failure_sequence/rf_task_2.h"
#include "task_sequences/rapi_failure_sequence/rf_task_to.h"

#include "serial.h"

Task_Result
rf_task_1(Controller *ctrl, OCPP_MessageID t_id)
{

#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RF_1\r");
#endif

    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_IN_PROGRESS,
            .task =
            {
                .type = TASK_PROCESS,
                .usart = RAPI_USART,
                .func = rf_task_2,
                .func_timeout = rf_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };

    ctrl->rapi._mod_booted = true;

    OCPP_ChargePointStatus status = CPS_Unavailable;
    OCPP_ChargePointErrorCode error = CPEC_EVCommunicationError;

    ctrl->memory.status = status;
    _controller_memory_store(&(ctrl->memory));

    _controller_ocpp_make_msg(&(ctrl->ocpp), ACT_STATUS_NOTIFICATION, &status, &error, NULL);
    _controller_ocpp_send_req(&(ctrl->ocpp), ACT_STATUS_NOTIFICATION);
    
    return res;
}
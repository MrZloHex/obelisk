#include "task_sequences/rapi_failure_sequence/rf_task_1.h"
#include "task_sequences/rapi_failure_sequence/rf_task_2.h"
#include "task_sequences/get_state_sequence/gs_task_2.h"

#include "serial.h"
#include "controller_rapi_msg.h"

Task_Result
rf_task_2(Controller *ctrl, OCPP_MessageID t_id)
{

#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RF_2\r");
#endif
    ctrl->ocpp._started = true;

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
                .func = gs_task_2,
                .func_timeout = rf_task_1,
                .genesis_time = HAL_GetTick()
            }
        }
    };

    _rapi_get_state_req(&(ctrl->rapi));
    if (_rapi_send_req(&(ctrl->rapi)) == CTRL_PTCL_PENDING)
    {
        res.type = TRES_WAIT;
        res.task.task.func = rf_task_2;
    }
    
    return res;
}
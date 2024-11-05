#include "task_sequences/rapi_failure_sequence/rf_task_to.h"

#include "serial.h"

Task_Result
rf_task_to(Controller *ctrl, OCPP_MessageID t_id)
{

#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "RF_TO\r");
#endif
    Task_Result res =
    {
        .type = TRES_NEXT,
        .task =
        {
            .type = WRAP_FINISHED,
        }
    };

    ctrl->ocpp._started = false;
    
    return res;
}
#include "task_sequences/stop_sequence/sts_task_to.h"
#include "task_sequences/stop_sequence/sts_task_1.h"
#include "task_sequences/stop_sequence/sts_task_4.h"

#include "serial.h"

Task_Result
sts_task_to(Controller *ctrl, OCPP_MessageID t_id)
{
    static uint8_t reps = 1;

#ifdef DEBUG
    uprintf(DBUG_UART, 1000, 10, "STS_TO\r");
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
                .func_timeout = sts_task_to,
                .genesis_time = HAL_GetTick()
            }
        }
    };

    if ((reps % MAX_MSG_REPS) == 0)
    {
        res.task.task.func = sts_task_4;
    }
    else
    {
        reps++;
        res.task.task.func = sts_task_1;
    }


    return res;
}




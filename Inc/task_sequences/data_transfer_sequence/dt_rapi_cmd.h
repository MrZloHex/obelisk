#ifndef __DATA_TRANSFER_SEQ_RAPI_CMD_H__ 
#define __DATA_TRANSFER_SEQ_RAPI_CMD_H__ 

#include "controller_task.h"
#include "controller.h"


Task_Result
dt_rapi_cmd(Controller *ctrl, OCPP_MessageID _id);

Task_Result
dt_rapi_cmd_to(Controller *ctrl, OCPP_MessageID _id);

Task_Result
dt_rapi_cmd_fin(Controller *ctrl, OCPP_MessageID _id);

#endif /* __DATA_TRANSFER_SEQ_TASK_RAPI_CMD_H__ */


#ifndef __HEARTBEAT_SEQ_TASK_TO_H__
#define __HEARTBEAT_SEQ_TASK_TO_H__

#include "controller_task.h"
#include "controller.h"

Task_Result
hb_task_to(Controller *ctrl, OCPP_MessageID t_id);

#endif /* __HEARTBEAT_TASK_TO_H__ */
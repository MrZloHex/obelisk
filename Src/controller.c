#include "controller.h"
#include "controller_lcd.h"
#include "serial.h"
#include "timer.h"
#include "controller_temperature.h"

#include "task_sequences/remote_start_sequence/rss_task_1.h"
#include "task_sequences/meter_values_sequence/mv_task_1.h"
#include "task_sequences/stop_sequence/sts_task_1.h"
#include "task_sequences/boot_sequence/bs_task_1.h"
#include "task_sequences/get_state_sequence/gs_task_1.h"
#include "task_sequences/heartbeat_sequence/hb_task_1.h"
#include "task_sequences/data_transfer_sequence/dt_task_1.h"

Controller_Result
controller_initialize
(
	Controller *controller,
	UART_HandleTypeDef *ocpp_uart,
	UART_HandleTypeDef *rapi_uart,
	TIM_HandleTypeDef *ocpp_tim,
	TIM_HandleTypeDef *rapi_tim,
	RTC_HandleTypeDef *rtc,
	I2C_HandleTypeDef *i2c,
	GPIO_TypeDef   	  *wp_gpio,
	uint16_t 	  wp_pin,
	ResetCause rc
)
{
	Controller_TaskSet_Result res = _controller_taskset_initialize(&(controller->task_set), MAX_TASKSET_CAPACITY);
	if (res != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, res);
	}

	_controller_ocpp_initialize(&(controller->ocpp), ocpp_uart, ocpp_tim, rtc, i2c, wp_gpio, wp_pin);
	_controller_rapi_initialize(&(controller->rapi), rapi_uart, rapi_tim);

	_controller_lcd_init(controller, i2c);
	_controller_memory_init(&(controller->memory), i2c);
	_controller_temp_init(i2c);

	if (rc != RC_INDEPENDENT_WATCHDOG_RESET)
	{
		controller->memory.fatal_err.type = 0;
		controller->memory.fatal_err.errors.ocpp_err = 0;
		_controller_memory_store(&(controller->memory));
	}

	// BASE TASKS

	Controller_TaskWrap wr;
	BS_TASK_WRAP((&wr));
	res = _controller_taskset_push(&(controller->task_set), wr);
	if (res != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, res);
	}

	GS_TASK_WRAP((&wr));
	res = _controller_taskset_push(&(controller->task_set), wr);
	if (res != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, res);
	}


	if (controller->memory.in_transaction)
	{
		STS_TASK_WRAP((&wr));
		res = _controller_taskset_push(&(controller->task_set), wr);
		if (res != CTRL_SET_OK)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, res);
		}
	}

	CONTROLLER_OKAY;
}

/**
 * @brief UPDATE MESSAGES ON OCPP UART
 * 
 * @param controller 
 * @return Controller_Result 
 */
static Controller_Result
_update_ocpp_uart(Controller *controller)
{
	Controller_TaskWrap wrap;
	Controller_TaskSet_Result tres;
	Controller_Protocol_Result pres;

	pres = controller_ocpp_update(&(controller->ocpp), &wrap);

	if (pres == CTRL_PTCL_PROCESSED)
	{
		tres = _controller_taskset_push(&(controller->task_set), wrap);
		if (tres != CTRL_SET_OK)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}
	}
	else
	{
		if (PTCL_FATAL(pres))
		{
			CONTROLLER_ERROR(CTRL_OCPP_ERR, ocpp_err, pres);
		}
	}

	CONTROLLER_OKAY;
}

/**
 * @brief UPDATE MESSAGES ON RAPI UART
 * 
 * @param controller 
 * @return Controller_Result 
 */
static Controller_Result
_update_rapi_uart(Controller * controller)
{
	
	Controller_TaskWrap wrap;
	Controller_TaskSet_Result tres;
	Controller_Protocol_Result pres;

	pres = controller_rapi_update(&(controller->rapi), &wrap);
	/* if message is processed, create a task */
	if (pres == CTRL_PTCL_PROCESSED)
	{
		tres = _controller_taskset_push(&(controller->task_set), wrap);
		if (tres != CTRL_SET_OK) 
		{ 
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}
	}
	else
	{
		if (PTCL_FATAL(pres)) 
		{
			CONTROLLER_ERROR(CTRL_RAPI_ERR, rapi_err, pres); 
		}
	}
	
	CONTROLLER_OKAY;
}

static Controller_Result
_update_meter_values(Controller * controller)
{
	#define METER_VALUES_TIMEOUT 10000
	Controller_TaskSet_Result tres;
	static Timer mv_timer;
	timer_set(&mv_timer, METER_VALUES_TIMEOUT, true);

	if (controller->memory.in_transaction && controller->memory.status == CPS_Charging)
	{ timer_start(&mv_timer); }
	else
	{ timer_stop (&mv_timer); }

	if (timer_timeout(&mv_timer))
	{
		Controller_TaskWrap mv_wrap;
		MV_TASK_WRAP((&mv_wrap));
		tres = _controller_taskset_push(&(controller->task_set), mv_wrap);
		if (tres != CTRL_SET_OK)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}
	}
	CONTROLLER_OKAY;
}

static Controller_Result
_update_status_notification(Controller *controller)
{
	#define STATUS_NOTIFICATION_TIMEOUT 30000
	Controller_TaskSet_Result tres;
	static Timer sn_timer;
	timer_set(&sn_timer, STATUS_NOTIFICATION_TIMEOUT, true);
	// if (!controller->memory.in_transaction) { timer_start(&sn_timer); }
	// else 								 { timer_stop (&sn_timer); }
	timer_start(&sn_timer);

	if (timer_timeout(&sn_timer))
	{
		Controller_TaskWrap sn_wrap;
		GS_TASK_WRAP((&sn_wrap));
		tres = _controller_taskset_push(&(controller->task_set), sn_wrap);
		if (tres != CTRL_SET_OK)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}
	}
	CONTROLLER_OKAY;
}

static Controller_Result
_update_charged_state(Controller *controller)
{

	#define CHARGED_TIMEOUT 60000
	static Timer charged_timer;
	timer_set(&charged_timer, CHARGED_TIMEOUT, false);

	if (controller->memory.in_transaction && controller->memory.status == CPS_Preparing)
	{ timer_start(&charged_timer); }
	else
	{ timer_stop(&charged_timer); }

	if (timer_timeout(&charged_timer))
	{
		Controller_TaskWrap sts_wrap;
		STS_TASK_WRAP((&sts_wrap));
		_controller_taskset_push(&(controller->task_set), sts_wrap);
	}
	CONTROLLER_OKAY;

}

Controller_Result
_update_heartbeat(Controller *controller)
{
	#define HEARTBEAT_TIMEOUT 60000
	Controller_TaskSet_Result tres;
	static Timer hb_timer;
	timer_set(&hb_timer, HEARTBEAT_TIMEOUT, true);
	timer_start(&hb_timer);

	if (timer_timeout(&hb_timer))
	{
		Controller_TaskWrap hb_wrap;
		HB_TASK_WRAP((&hb_wrap));
		tres = _controller_taskset_push(&(controller->task_set), hb_wrap);
		if (tres != CTRL_SET_OK)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}
	}
	CONTROLLER_OKAY;
}

Controller_Result
_update_tasks(Controller *controller)
{
	Controller_TaskSet_Result tres;
	// IF THERE ARE NO FACES (TASKS SORRY) - RETURN
	if (controller->task_set.size == 0)
	{
		CONTROLLER_OKAY;
	}

	tres = _controller_taskset_iterate(&(controller->task_set));
	if (tres != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
	}

	Controller_TaskWrap task_wrap;

	for (size_t i = 0; i < controller->task_set.size; ++i)
	{
		tres = _controller_taskset_next(&(controller->task_set), &task_wrap);
		if (tres != CTRL_SET_OK)
		{
			_controller_taskset_esc_iter(&(controller->task_set));
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
		}

		if (task_wrap.type == WRAP_EMPTY) { continue; }

		#define K_TASK_TIMEOUT 30000
		if (task_wrap.task.genesis_time + K_TASK_TIMEOUT <= HAL_GetTick())
		{
			task_wrap.type = WRAP_TIMEOUT;
			task_wrap.task.func = task_wrap.task.func_timeout;
			task_wrap.task.type = TASK_TIMEOUT;
		}

		if (task_wrap.task.type == TASK_PROCESS)
		{
			if (task_wrap.task.usart == OCPP_USART)
			{
				if (!controller->ocpp.is_response)								{ continue; }
				if (!_ocpp_get_resp(&(controller->ocpp), task_wrap.task.id))	{ continue; }
			}
			else if (task_wrap.task.usart == RAPI_USART)
			{
				if (controller->rapi.pending) { continue; }
			}
		}
		// __debug_taskset_print(&(controller->task_set));
		if (task_wrap.task.func == NULL)
		{
			CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, CTRL_SET_NULLPTR);
		}

		// EXECUTE TASK AND SET NEXT TASK
		Task_Result task_res = task_wrap.task.func(controller, task_wrap.task.trigger_id);
		if (task_res.type == TRES_NEXT)
		{
			tres = _controller_taskset_update_task(&(controller->task_set), task_res.task);
			if (tres != CTRL_SET_OK)
			{
				_controller_taskset_esc_iter(&(controller->task_set));
				CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
			}
			// __debug_taskset_print(&(controller->task_set));
			break;
		}
	}

	_controller_taskset_esc_iter(&(controller->task_set));
	// CHECK FOR FINISHED TASKS AND DELETE THEM
	tres = _controller_taskset_pop(&(controller->task_set));
	if (tres != CTRL_SET_OK)
	{
		CONTROLLER_ERROR(CTRL_TSET_ERR, tset_err, tres);
	}
	CONTROLLER_OKAY;
}








Controller_Result
controller_update(Controller *controller)
{
	_controller_lcd_update(controller);

	Controller_Result cres;

	cres = _update_ocpp_uart(controller);
	if (cres.type != CTRL_OK) return cres;

	cres = _update_rapi_uart(controller);
	if (cres.type != CTRL_OK) return cres;

	cres = _update_meter_values(controller);
	if (cres.type != CTRL_OK) return cres;

	cres = _update_status_notification(controller);
	if (cres.type != CTRL_OK) return cres;

	cres = _update_charged_state(controller);
	if (cres.type != CTRL_OK) return cres;

	cres = _update_heartbeat(controller);
	if (cres.type != CTRL_OK) return cres;

	cres = _update_tasks(controller);
	if (cres.type != CTRL_OK) return cres;

	CONTROLLER_OKAY;
}

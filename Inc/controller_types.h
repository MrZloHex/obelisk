/**
  ******************************************************************************
  * @file    controller_types.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_TYPES_H__
#define __CONTROLLER_TYPES_H__

#define DEBUG
#define DBUG_UART &huart2

#include "ocpp_types.h"
#include "stdbool.h"

typedef enum Controller_ResultType_E
{
	CTRL_OK                 = 0x0U,
	CTRL_ERR                = 0x1U,
	CTRL_TSET_ERR			= 0x2U,
	CTRL_OCPP_ERR 			= 0x3U,
	CTRL_RAPI_ERR 			= 0x4U,
	CTRL_TASK_ERR			= 0x5U
} Controller_ResultType;

typedef enum Controller_TaskSet_Result_E
{
	CTRL_SET_OK,
	CTRL_SET_ALLOC_ERR,
	CTRL_SET_OVERFLOW,
	CTRL_SET_EMPTY,
	CTRL_SET_NULLPTR,
	CTRL_SET_IN_ITER,
	CTRL_SET_NOT_ITER
} Controller_TaskSet_Result;

#define PTCL_FATAL(__RES__) (__RES__ == CTRL_PTCL_OVER_RESP || __RES__ == CTRL_PTCL_NULL_PTR)

typedef enum Controller_Protocol_Result_E
{
	CTRL_PTCL_OK			= 0,
	CTRL_PTCL_HAL_ERROR		= 1,
	CTRL_PTCL_HAL_BUSY		= 2,
	CTRL_PTCL_HAL_TIMEOUT	= 3,
	CTRL_PTCL_ACC_BUF_FULL	= 4,
	CTRL_PTCL_PRC_BUF_FULL	= 5,
	CTRL_PTCL_ACC_BUF_EMPT  = 6,
	CTRL_PTCL_PRC_BUF_EMPT  = 7,
	CTRL_PTCL_NON_VALID_MSG	= 8,
	CTRL_PTCL_UNKNOWN_MSG	= 9,
	CTRL_PTCL_NO_SUCH_MSG	= 10,
	CTRL_PTCL_NULL_PTR		= 11,
	CTRL_PTCL_OVER_RESP		= 12,
	CTRL_PTCL_RESPONSE		= 13,
	CTRL_PTCL_PENDING       = 14,
	CTRL_PTCL_WAITING		= 15,
	CTRL_PTCL_RECEIVED		= 16,
	CTRL_PTCL_PROCESSED		= 17
} Controller_Protocol_Result;

typedef union Controller_Errors_U
{
	Controller_TaskSet_Result	tset_err;
	Controller_Protocol_Result	ocpp_err;
	Controller_Protocol_Result	rapi_err;
} Controller_Errors;

typedef struct Controller_Result_S
{
	Controller_ResultType	type;
	Controller_Errors 		errors;
} Controller_Result;

typedef enum ResetCause_E
{
    RC_UNKNOWN = 0,
    RC_LOW_POWER_RESET,
    RC_WINDOW_WATCHDOG_RESET,
    RC_INDEPENDENT_WATCHDOG_RESET,
    RC_SOFTWARE_RESET,
    RC_POWER_ON_POWER_DOWN_RESET,
    RC_EXTERNAL_RESET_PIN_RESET
} ResetCause;

typedef struct Controller_TaskWrap_S Controller_TaskWrap;

#define CONTROLLER_OKAY													Controller_Result __res =						\
																		{												\
																			.type = CTRL_OK								\
																		};												\
																		return __res;

#define CONTROLLER_ERROR(__err_type__, __err_field__, __error__) 		Controller_Result __res = 						\
																		{												\
																			.type = __err_type__,						\
																			.errors = { .__err_field__ = __error__ }	\
																		};												\
																		return __res;



#define CONTROLLER_OCPP_ERROR(__error__)								CONTROLLER_ERROR(CTRL_OCPP_ERR, ocpp_err, __error__)
#define CONTROLLER_RAPI_ERROR(__error__)								CONTROLLER_ERROR(CTRL_RAPI_ERR, rapi_err, __error__)

#define CONTROLLER_TASK_RESULT(__task__) 								Controller_TaskResult __res =					\
																		{												\
																			.type = CTRL_OK, .task = __task__			\
																		};												\
																		return __res;

#endif /* __CONTROLLER_TYPES_H__ */

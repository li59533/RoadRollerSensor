/**
 **************************************************************************************************
 * @file        vibrate_task.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _VIBRATE_TASK_H_
#define _VIBRATE_TASK_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "osal.h"
#include "self_def.h"
/**
 * @addtogroup    vibrate_task_Modules 
 * @{  
 */

/**
 * @defgroup      vibrate_task_Exported_Macros 
 * @{  
 */
#define VIBRATE_TASK_LOOP_TEST_EVENT		0X0001
#define VIBRATE_TASK_INIT_EVENT				0X0002
#define VIBRATE_TASK_CALC_EVENT				0X0004
#define VIBRATE_TASK_STATUS_CHECK_EVENT		0X0008

/**
 * @}
 */

/**
 * @defgroup      vibrate_task_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      vibrate_task_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      vibrate_task_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      vibrate_task_Exported_Functions 
 * @{  
 */
void VibrateTask_Init(uint8_t taskId);
osal_event_t VibrateTask_Process(uint8_t taskid,osal_event_t events);
void VibrateTask_Send_Event(osal_event_t events);
void VibrateTask_Timer_Start_Event(osal_event_t event_flag,uint32_t timeout);
void VibrateTask_Clear_Event(osal_event_t events);
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
#endif

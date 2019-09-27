/**
 **************************************************************************************************
 * @file        vibrate_task.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "vibrate_task.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "ad7988.h"
#include "clog.h"
/**
 * @addtogroup    vibrate_task_Modules 
 * @{  
 */

/**
 * @defgroup      vibrate_task_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      vibrate_task_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      vibrate_task_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      vibrate_task_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      vibrate_task_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      vibrate_task_Public_Variables 
 * @brief         
 * @{  
 */
uint8_t g_VibrateTask_Id = 0;
/**
 * @}
 */

/**
 * @defgroup      vibrate_task_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      vibrate_task_Functions 
 * @brief         
 * @{  
 */
void VibrateTask_Init(uint8_t taskId)
{
    g_VibrateTask_Id = taskId;
	VibrateTask_Send_Event(VIBRATE_TASK_INIT_EVENT);
}

osal_event_t VibrateTask_Process(uint8_t taskid,osal_event_t events)
{
    if (events & VIBRATE_TASK_INIT_EVENT)
    {
		DEBUG("AD7988 Init\r\n");
		AD7988_Init();
		AD7988_ParamInit();
        return events ^ VIBRATE_TASK_INIT_EVENT;
    }	
	if (events & VIBRATE_TASK_CALC_EVENT)
    {
		DEBUG("AD7988 Startcalc\r\n");
		AD7988_Calc_Process();
        return events ^ VIBRATE_TASK_CALC_EVENT;
    }	
	
	if (events & VIBRATE_TASK_STATUS_CHECK_EVENT)
    {
		DEBUG("AD7988 Check status\r\n");
		AD7988_Status_CheckProcess();
        return events ^ VIBRATE_TASK_STATUS_CHECK_EVENT;
    }	
    if (events & VIBRATE_TASK_LOOP_TEST_EVENT)
    {
		OS_Timer_Start(g_VibrateTask_Id, VIBRATE_TASK_LOOP_TEST_EVENT,1000);	
        return events ^ VIBRATE_TASK_LOOP_TEST_EVENT;
    }
    return 0;
}

void VibrateTask_Send_Event(osal_event_t events)
{
    OS_Events_Set(g_VibrateTask_Id,events);
}

void VibrateTask_Timer_Start_Event(osal_event_t event_flag,uint32_t timeout)
{
	OS_Timer_Start(g_VibrateTask_Id, event_flag,timeout);	
}

void VibrateTask_Clear_Event(osal_event_t events)
{

}
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */


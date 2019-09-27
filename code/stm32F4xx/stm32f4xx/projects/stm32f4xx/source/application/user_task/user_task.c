/**
 **************************************************************************************************
 * @file        user_task.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "self_def.h"
#include "osal.h"
#include "clog.h"
#include "user_task.h"
#include "stm32_bsp_conf.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
#include "t_4_20mv.h"
/**
 * @addtogroup    user_task_Modules 
 * @{  
 */

/**
 * @defgroup      user_task_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      user_task_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      user_task_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      user_task_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      user_task_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      user_task_Public_Variables 
 * @brief         
 * @{  
 */
uint8_t g_UserTask_Id = 0;
/**
 * @}
 */

/**
 * @defgroup      user_task_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      user_task_Functions 
 * @brief         
 * @{  
 */
void UserTask_Init(uint8_t taskId)
{
    g_UserTask_Id = taskId;
    UserTask_Send_Event(USER_TASK_LOOP_EVENT);	
	BSP_LED_Blink(BSP_LED1, 0 , 10, 500);
	BSP_LED_Blink(BSP_LED2, 0 , 50, 500);
}

osal_event_t UserTask_Process(uint8_t taskid,osal_event_t events)
{
    if (events & USER_TASK_LOOP_EVENT)
    {
		uint16_t sec = 0; 
		sec = OS_Clock_GetSeconds();
		DEBUG("TASK_1\r\n");
		DEBUG("USER_TASK_LOOP_EVENT:%d\r\n",sec);
		
		OS_Timer_Start(g_UserTask_Id, USER_TASK_LOOP_EVENT,1000);
			
        return events ^ USER_TASK_LOOP_EVENT;
    }
	
	if (events & USER_TASK_T420MV_CALC_EVENT)
    {			
		T_4_20mv_CalcProcess();
        return events ^ USER_TASK_T420MV_CALC_EVENT;
    }
	
    return 0;
}

void UserTask_Send_Event(osal_event_t events)
{
    OS_Events_Set(g_UserTask_Id,events);
}

void UserTask_Clear_Event(osal_event_t events)
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


/**
 **************************************************************************************************
 * @file        bsp_init.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "bsp_init.h"
#include "stm32_bsp_conf.h"
#include "main.h"
#include "clog.h"
#include "malloc.h"
/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    bsp_init_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_init_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_init_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      bsp_init_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_init_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      bsp_init_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      bsp_init_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_init_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**s
 * @}
 */


/**
 * @defgroup      bsp_init_Functions 
 * @brief         
 * @{  
 */

void Board_Init(void)
{
	DEBUG("BoardInit\r\n");

	BSP_Systick_Init();  // open the 1ms systick ...
	
}
void BSP_Init(void)
{
	DEBUG("BSP_Init Start\r\n");
	BSP_USART_Open(BSP_USART1,0);
	BSP_ADC_Open(BSP_ADC1,0);
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池
	
	DEBUG("BSP_Init Complete\r\n");
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


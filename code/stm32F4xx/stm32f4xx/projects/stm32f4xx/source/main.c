/**
 **************************************************************************************************
 * @file        main.c
 * @author
 * @version
 * @date        4/25/2017
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "string.h"
#include "osal.h"
#include "system_init.h"
#include "main.h"
#include "bsp_init.h"
#include "clog.h"
/**
 * @addtogroup    XXX 
 * @{  
 */

/**	
 * @addtogroup    main_Modules 
 * @{  
 */

/**
 * @defgroup      main_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       main_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       main_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      main_Functions 
 * @brief         
 * @{  
 */
int main()
{
    Board_Init(); // init systick and system clock
    
    __enable_irq(); // open irq
    
    System_Load();  // init bsp peripheral and load system param
    
    OS_Init(); // init osal
    
    OS_Start(); // while(1)
    
    return 0;
}


void Error_Handler(void)
{
	while(1)
	{
		DEBUG(0,"Error_Handler\r\n");
	};
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


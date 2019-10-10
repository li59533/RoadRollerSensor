/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_systick.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "stm32f4xx_bsp_conf.h"

/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    stm32f4xx_bsp_systick_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_systick_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_systick_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_systick_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static uint32_t s_Systick_Ticks = 0;
/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_Functions 
 * @brief         
 * @{  
 */
void BSP_Systick_Init(void) // if using ths HAL ,please using the HAl_Init();
{
	/* SysTick end of count event each 1ms */
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	
}

void BSP_SYSTICK_IncTick(void)
{
    s_Systick_Ticks++;
}

uint32_t BSP_SYSTICK_GetTick(void)
{
    return s_Systick_Ticks;
}


void BSP_Systick_Delayms(uint32_t ticks) // block delay
{
    uint32_t tickstart = 0;
    tickstart = BSP_SYSTICK_GetTick();
    while ((BSP_SYSTICK_GetTick() - tickstart) < ticks) 
    {
    }
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


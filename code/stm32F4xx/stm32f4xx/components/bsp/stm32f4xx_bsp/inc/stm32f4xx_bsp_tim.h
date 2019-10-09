/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_tim.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _STM32F4XX_BSP_TIM_H_
#define _STM32F4XX_BSP_TIM_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    stm32f4xx_bsp_tim_Modules 
 * @{  
 */
#define BSP_TIM_COUNT	1
#define BSP_TIM1 		0
#define BSP_TIM2		1
/**
 * @defgroup      stm32f4xx_bsp_tim_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_tim_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_tim_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_tim_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_tim_Exported_Functions 
 * @{  
 */
void BSP_Tim_Open(uint8_t BSP_TIMx,uint8_t *userparam);

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

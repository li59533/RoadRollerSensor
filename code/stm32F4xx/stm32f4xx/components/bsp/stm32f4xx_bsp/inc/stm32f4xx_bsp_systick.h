/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_systick.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _STM32F4XX_BSP_SYSTICK_H_
#define _STM32F4XX_BSP_SYSTICK_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */

/**
 * @addtogroup    stm32f4xx_bsp_systick_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_systick_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_systick_Exported_Functions 
 * @{  
 */
void BSP_Systick_Init(void); // if using ths HAL ,please using the HAl_Init();
void BSP_Systick_Delayms(uint32_t delay); // block delay


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


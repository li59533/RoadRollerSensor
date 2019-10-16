/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_flash.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _STM32F4XX_BSP_FLASH_H_
#define _STM32F4XX_BSP_FLASH_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    stm32f4xx_bsp_flash_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_flash_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_Exported_Functions 
 * @{  
 */
int8_t BSP_Flash_WriteBytes(uint32_t AddrStart,uint8_t *buf,uint16_t len);
void BSP_Flash_ReadBytes(uint32_t AddrStart,uint16_t len, uint8_t *buf);
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

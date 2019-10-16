/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_flash.c
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
 * @addtogroup    stm32f4xx_bsp_flash_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_flash_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_flash_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_flash_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_flash_Functions 
 * @brief         
 * @{  
 */

int8_t BSP_Flash_WriteBytes(uint32_t AddrStart,uint8_t *buf,uint16_t len)
{
	uint16_t i = 0;
	
	if(FLASH_GetStatus() == FLASH_BUSY)
	{
		return -1;
	}
    FLASH_Unlock();
	
	FLASH_EraseSector(FLASH_Sector_10,VoltageRange_3);
	
	for(i = 0; i < len; i++)
	{
		FLASH_ProgramByte(AddrStart + i, buf[i]);
		while(FLASH_GetStatus() == FLASH_BUSY);
	}
    FLASH_Lock();	
	
	return 0;
}

void BSP_Flash_ReadBytes(uint32_t AddrStart,uint16_t len, uint8_t *buf)
{
	uint16_t i = 0 ;
	for(i = 0; i < len ; i ++)
	{
		buf[i] = *(uint8_t *)(AddrStart + i );
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


/**
 **************************************************************************************************
 * @file        ad7988_port.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "ad7988_port.h"
#include "stm32f4xx_bsp_conf.h"
/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    ad7988_port_Modules 
 * @{  
 */

/**
 * @defgroup      ad7988_port_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_port_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      ad7988_port_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_port_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      ad7988_port_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      ad7988_port_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_port_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_port_Functions 
 * @brief         
 * @{  
 */
void AD7988_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; 
	
	GPIO_StructInit(&GPIO_InitStruct);		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);		
	
	BSP_SPI_Open(BSP_SPI1,0);
	
	
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


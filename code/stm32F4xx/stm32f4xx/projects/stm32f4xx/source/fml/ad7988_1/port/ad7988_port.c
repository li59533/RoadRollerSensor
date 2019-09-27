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


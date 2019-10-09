/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_spi.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _STM32F4XX_BSP_SPI_H_
#define _STM32F4XX_BSP_SPI_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    stm32f4xx_bsp_spi_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_Exported_Macros 
 * @{  
 */
#define BSP_SPI_COUNT 			1

#define BSP_SPI1				0


/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_spi_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_Exported_Functions 
 * @{  
 */
void BSP_SPI_Open(uint8_t BSP_SPIx, uint8_t *userparams);


void BSP_SPI_Start(uint8_t BSP_SPIx);

void BSP_SPI1_DMA_IRQHandler(void);
void BSP_SPI1_IRQHandler(void);
uint16_t SPI1_ReadWriteByte(uint16_t TxData);

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

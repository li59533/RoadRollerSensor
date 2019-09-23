/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_usart.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _STM32F4XX_BSP_USART_H_
#define _STM32F4XX_BSP_USART_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
#include "stm32f4xx_hal.h"
/**
 * @addtogroup    stm32f4xx_bsp_usart_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Exported_Macros 
 * @{  
 */
#define BSP_UART_COUNT			1
#define BSP_USART1				0
#define	BSP_USART2				1 


#define BSP_USART1_TXBUF_SIZE 	300		
#define BSP_USART1_RXBUF_SIZE	300
#define BSP_USART2_TXBUF_SIZE	300
#define BSP_USART2_RXBUF_SIZE	300
/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_usart_Exported_Types 
 * @{  
 */
typedef struct
{
    uint8_t *pData;
    uint16_t Size;
    uint16_t Count;
    uint16_t In;
    uint16_t Out;
}USART_Buf_t;

typedef struct
{
    USART_TypeDef   *Instance;
    USART_Buf_t   TxBuf;
    uint16_t      TxCount;
    USART_Buf_t   RxBuf;
}STM32F4xxUSART_Instance_t;
/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Exported_Variables 
 * @{  
 */
extern USART_HandleTypeDef husart;
extern DMA_HandleTypeDef hdmatx;
extern DMA_HandleTypeDef hdmarx;
extern GPIO_InitTypeDef GPIO_InitStruct;

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Exported_Functions 
 * @{  
 */
void BSP_USART_Open(uint8_t BSP_USARTx, uint8_t *userparams);
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



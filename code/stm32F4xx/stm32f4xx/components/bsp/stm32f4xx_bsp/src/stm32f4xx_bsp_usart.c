/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_usart.c
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
 * @addtogroup    stm32f4xx_bsp_usart_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_usart_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_usart_Private_Variables 
 * @brief         
 * @{  
 */
static uint8_t USART1_Tx_Buf[BSP_USART1_TXBUF_SIZE] = { 0 };
static uint8_t USART1_Rx_Buf[BSP_USART1_RXBUF_SIZE] = { 0 };
static uint8_t USART2_Tx_Buf[BSP_USART2_TXBUF_SIZE] = { 0 };
static uint8_t USART2_Rx_Buf[BSP_USART2_RXBUF_SIZE] = { 0 };

static STM32F4xxUSART_Instance_t STM32F4xxUSART_Instance[BSP_UART_COUNT] = {
    /* USART1 */
    {
        /* USART_TypeDef */
        .Instance = USART1,
        /* tx buf */
        .TxBuf.pData = USART1_Tx_Buf,
        .TxBuf.Size = sizeof(USART1_Tx_Buf) / sizeof(USART1_Tx_Buf[0]),
        .TxBuf.Count = 0,
        .TxBuf.In = 0,
        .TxBuf.Out = 0,
        /* rx buf */
        .RxBuf.pData = USART1_Rx_Buf,
        .RxBuf.Size = sizeof(USART1_Rx_Buf) / sizeof(USART1_Rx_Buf[0]),
        .RxBuf.Count = 0,
        .RxBuf.In = 0,
        .RxBuf.Out = 0,
    },
};


/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_usart_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Functions 
 * @brief         
 * @{  
 */
USART_HandleTypeDef husart = { 0 };
DMA_HandleTypeDef hdmatx = { 0 };
DMA_HandleTypeDef hdmarx = { 0 };
GPIO_InitTypeDef GPIO_InitStruct = { 0 };
void BSP_USART_Open(uint8_t BSP_USARTx, uint8_t *userparams)
{

	switch(BSP_USARTx)
	{
		case BSP_USART1:
		{	
			// ----CLK ENABLE----------
			__HAL_RCC_GPIOA_CLK_ENABLE();
			__HAL_RCC_USART1_CLK_ENABLE();
			__HAL_RCC_DMA2_CLK_ENABLE();
			// ------------------------
			
			// ----GPIO INIT-----------
			GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
			GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			// ------------------------
			
			hdmatx.Instance = DMA2_Stream7;
			hdmatx.Init.Channel = DMA_CHANNEL_4;
			hdmatx.Init.Direction = DMA_MEMORY_TO_PERIPH;
			hdmatx.Init.PeriphInc = DMA_PINC_DISABLE;
			hdmatx.Init.MemInc = DMA_MINC_ENABLE;
			hdmatx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			hdmatx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
			hdmatx.Init.Mode = DMA_NORMAL;
			hdmatx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
			hdmatx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
			
			hdmarx.Instance = DMA2_Stream5;
			hdmarx.Init.Channel = DMA_CHANNEL_4;
			hdmarx.Init.Direction = DMA_PERIPH_TO_MEMORY;
			hdmarx.Init.PeriphInc = DMA_PINC_DISABLE;
			hdmarx.Init.MemInc = DMA_MINC_ENABLE;
			hdmarx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			hdmarx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
			hdmarx.Init.Mode = DMA_NORMAL;
			hdmarx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
			hdmarx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
			
			husart.Instance = USART1;
			husart.Init.BaudRate = 115200;
			husart.Init.WordLength = USART_WORDLENGTH_8B;
			husart.Init.StopBits = USART_STOPBITS_1;
			husart.Init.Parity = USART_PARITY_NONE;
			husart.Init.Mode = USART_MODE_TX_RX;
			husart.Init.CLKPolarity = USART_POLARITY_LOW;
			husart.Init.CLKPhase = USART_PHASE_1EDGE;
			husart.Init.CLKLastBit = USART_LASTBIT_DISABLE;
			
			husart.pTxBuffPtr = USART1_Tx_Buf;
			husart.TxXferSize = BSP_USART1_TXBUF_SIZE;
			husart.pRxBuffPtr = USART1_Rx_Buf;
			husart.RxXferSize = BSP_USART1_RXBUF_SIZE;
			husart.hdmarx = &hdmarx;
			husart.hdmatx = &hdmatx;

			HAL_DMA_Init(husart.hdmarx); 
			HAL_DMA_Init(husart.hdmatx); 

			HAL_USART_Init(&husart);

			HAL_NVIC_SetPriority(USART1_IRQn, 2, 2);
			HAL_NVIC_EnableIRQ(USART1_IRQn);
			
			HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 2, 2);
			HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
			
			//__HAL_USART_ENABLE_IT(&husart,USART_IT_IDLE);
			HAL_USART_Receive_DMA(&husart,USART1_Rx_Buf,BSP_USART1_RXBUF_SIZE);
			HAL_USART_Transmit_DMA( &husart, USART1_Tx_Buf, BSP_USART1_TXBUF_SIZE);
			
		}
		break;
		case BSP_USART2:
		{
		}
		break;
		default:break;
	}
	
}


void BSP_USART_Close(uint8_t BSP_USARTx)
{
	
}

void BSP_USART_WriteBytes(uint8_t BSP_USARTx,uint8_t* pBuf,uint16_t length)
{
	
}
	
uint16_t BSP_USART_ReadBytes(uint8_t BSP_USARTx,uint8_t* pBuf,uint16_t length)
{
	
}



//void     BSP_USART_IRQHandler(uint8_t BSP_USARTx);
void BSP_USART_TxDMA_IRQHandler(uint8_t BSP_USARTx)
{
	
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


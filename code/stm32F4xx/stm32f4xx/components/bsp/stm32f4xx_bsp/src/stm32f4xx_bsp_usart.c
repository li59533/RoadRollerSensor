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
#include "clog.h"
#include "app_conf.h"
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

static uint8_t USART6_Tx_Buf[BSP_USART6_TXBUF_SIZE] = { 0 };
static uint8_t USART6_Rx_Buf[BSP_USART6_RXBUF_SIZE] = { 0 };



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
    /* USART6 */
    {
        /* USART_TypeDef */
        .Instance = USART6,
        /* tx buf */
        .TxBuf.pData = USART6_Tx_Buf,
        .TxBuf.Size = sizeof(USART6_Tx_Buf) / sizeof(USART6_Tx_Buf[0]),
        .TxBuf.Count = 0,
        .TxBuf.In = 0,
        .TxBuf.Out = 0,
        /* rx buf */
        .RxBuf.pData = USART6_Rx_Buf,
        .RxBuf.Size = sizeof(USART6_Rx_Buf) / sizeof(USART6_Rx_Buf[0]),
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
static uint16_t bsp_usartTx_getqueuefreespace(uint8_t BSP_USARTx);
static int8_t bsp_usartTx_enqueue(uint8_t BSP_USARTx, uint8_t * buf,uint16_t len);
static uint16_t bsp_getusartTx_count(uint8_t BSP_USARTx);
static void bsp_usart_485en_init(void);  // init the 485 EN pin
static void bsp_usart_485en_T(uint8_t BSP_USARTx);
static void bsp_usart_485en_R(uint8_t BSP_USARTx);

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_usart_Functions 
 * @brief         
 * @{  
 */

void BSP_USART_Open(uint8_t BSP_USARTx, uint8_t *userparams)
{
	USART_InitTypeDef USART_InitStruct  = { 0 }; 
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; 
	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	DMA_InitTypeDef DMA_InitStruct = { 0 };
	
	bsp_usart_485en_init(); // init 485 EN pin GPIO 
	
	switch(BSP_USARTx)
	{
		case BSP_USART1:
		{	
			// ----CLK ENABLE----------
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
			//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
			// ------------------------
			
			// ----GPIO INIT-----------
			GPIO_StructInit(&GPIO_InitStruct);		
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_10;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_Init(GPIOA,&GPIO_InitStruct);		
			/* GPIO Alternate functions configuration function ****************************/
			GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1);			
			GPIO_PinAFConfig(GPIOA,GPIO_PinSource10, GPIO_AF_USART1);	
			// ------------------------
			/* Initialization and Configuration functions *********************************/
			USART_StructInit(&USART_InitStruct);
			USART_InitStruct.USART_BaudRate = 115200;
			USART_Init(USART1, &USART_InitStruct);
			// ---------DMA Init -------------------
			
			if(BSP_USART1_TX_DMA_ACTIVE)
			{
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); //DMA Clock open
				DMA_StructInit(&DMA_InitStruct);
				DMA_InitStruct.DMA_BufferSize = BSP_USART1_TXBUF_SIZE;
				DMA_InitStruct.DMA_Channel = DMA_Channel_4;
				DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
				DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)USART1_Tx_Buf;
				DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
				DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
				DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
				DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &USART1->DR;
				DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
				DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
				DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
				
				DMA_Init( DMA2_Stream7,&DMA_InitStruct);
				// ---------OPEN IT-----------
				NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream7_IRQn;
				NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
				NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
				NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStruct);
				DMA_ITConfig(DMA2_Stream7, DMA_IT_TC,  ENABLE);
				// ---------------------------
				
				DMA_Cmd(DMA2_Stream7,DISABLE);
				
				USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
			}
			if(BSP_USART1_RX_DMA_ACTIVE)
			{
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); //DMA Clock open
				DMA_StructInit(&DMA_InitStruct);
				DMA_InitStruct.DMA_BufferSize = BSP_USART1_RXBUF_SIZE;
				DMA_InitStruct.DMA_Channel = DMA_Channel_4;
				DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
				DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)USART1_Rx_Buf;
				DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
				DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
				DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
				DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &USART1->DR;
				DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
				DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
				DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
				
				DMA_Init( DMA2_Stream5,&DMA_InitStruct);

				DMA_Cmd(DMA2_Stream5,ENABLE);
				
				USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
			}
			
			
			
			// ---------OPEN IT---------------------
			//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			
			if(BSP_USART1_IDEL_ACTIVE)
			{
				USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
			}
			// -------Init the NVIC-----------------
			NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStruct);
			// ------USART Start -------
			USART_Cmd(USART1,ENABLE);
		}
		break;
		case BSP_USART6:
		{
		// ----CLK ENABLE----------
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
			//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
			// ------------------------
			
			// ----GPIO INIT-----------
			GPIO_StructInit(&GPIO_InitStruct);		
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_Init(GPIOC,&GPIO_InitStruct);		
			/* GPIO Alternate functions configuration function ****************************/
			GPIO_PinAFConfig(GPIOC,GPIO_PinSource6, GPIO_AF_USART6);			
			GPIO_PinAFConfig(GPIOC,GPIO_PinSource7, GPIO_AF_USART6);	
			// ------------------------
			/* Initialization and Configuration functions *********************************/
			USART_StructInit(&USART_InitStruct);
			USART_InitStruct.USART_BaudRate = 115200;
			USART_Init(USART6, &USART_InitStruct);
			// ---------DMA Init -------------------
			
			if(BSP_USART6_TX_DMA_ACTIVE)
			{
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); //DMA Clock open
				DMA_StructInit(&DMA_InitStruct);
				DMA_InitStruct.DMA_BufferSize = BSP_USART6_TXBUF_SIZE;
				DMA_InitStruct.DMA_Channel = DMA_Channel_5;
				DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
				DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)USART6_Tx_Buf;
				DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
				DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
				DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
				DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &USART6->DR;
				DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
				DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
				DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
				
				DMA_Init( DMA2_Stream6,&DMA_InitStruct);
				// ---------OPEN IT-----------
				NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream6_IRQn;
				NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
				NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
				NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStruct);
				DMA_ITConfig(DMA2_Stream6, DMA_IT_TC,  ENABLE);
				// ---------------------------
				
				DMA_Cmd(DMA2_Stream6,DISABLE);
				
				USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
			}
			if(BSP_USART6_RX_DMA_ACTIVE)
			{
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); //DMA Clock open
				DMA_StructInit(&DMA_InitStruct);
				DMA_InitStruct.DMA_BufferSize = BSP_USART6_RXBUF_SIZE;
				DMA_InitStruct.DMA_Channel = DMA_Channel_5;
				DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
				DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)USART6_Rx_Buf;
				DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
				DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
				DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
				DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &USART6->DR;
				DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
				DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
				DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
				
				DMA_Init( DMA2_Stream1,&DMA_InitStruct);

				DMA_Cmd(DMA2_Stream1,ENABLE);
				
				USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
			}
			
			
			
			// ---------OPEN IT---------------------
			//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			
			if(BSP_USART6_IDEL_ACTIVE)
			{
				USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);
			}
			// -------Init the NVIC-----------------
			NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStruct);
			// ------USART Start -------
			USART_Cmd(USART6,ENABLE);
		}
		break;
		default:break;
	}
	
}

static void bsp_usart_485en_init(void)  // init the 485 EN pin
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; 
	// ----CLK ENABLE----------
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	// ----GPIO INIT-----------
	GPIO_StructInit(&GPIO_InitStruct);		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;   // Usart1  use PA11
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;   // Usart1  use PA11
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);	
	
	GPIO_ResetBits( GPIOA, GPIO_Pin_11);	
	GPIO_ResetBits( GPIOC, GPIO_Pin_8);	
}

static void bsp_usart_485en_R(uint8_t BSP_USARTx)
{
	switch(BSP_USARTx)
	{
		case BSP_USART1: GPIO_SetBits( GPIOA, GPIO_Pin_11);	break;
		case BSP_USART6: GPIO_SetBits( GPIOC, GPIO_Pin_8); break;
		default:break;
	}
}

static void bsp_usart_485en_T(uint8_t BSP_USARTx)
{
	switch(BSP_USARTx)
	{
		case BSP_USART1: GPIO_ResetBits( GPIOA, GPIO_Pin_11); break;
		case BSP_USART6: GPIO_ResetBits( GPIOC, GPIO_Pin_8); break;
		default:break;
	}
}


void BSP_USART_Close(uint8_t BSP_USARTx)
{
	
}


void BSP_USART_WriteBytes_DMA(uint8_t BSP_USARTx,uint8_t* pbuf,uint16_t len)
{
	switch(BSP_USARTx)
	{
		case BSP_USART1:
		{
			bsp_usartTx_enqueue(BSP_USART1,pbuf,len);
			if(bsp_getusartTx_count(BSP_USART1) > 0)
			{
				DMA_Cmd(DMA2_Stream7,DISABLE);	
				bsp_usart_485en_T(BSP_USART1);				
				if(STM32F4xxUSART_Instance[BSP_USART1].TxBuf.In < STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out)
				{
					DMA2_Stream7->NDTR = STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Size - STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out;					
				}
				else
				{
					DMA2_Stream7->NDTR = STM32F4xxUSART_Instance[BSP_USART1].TxBuf.In - STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out;
				}
				
				STM32F4xxUSART_Instance[BSP_USART1].TxCount = DMA2_Stream7->NDTR;
				DMA2_Stream7->M0AR = (uint32_t)(STM32F4xxUSART_Instance[BSP_USART1].TxBuf.pData + STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out);
				DMA_Cmd(DMA2_Stream7,ENABLE);	
				DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
				DMA_ITConfig(DMA2_Stream7, DMA_IT_TC,  ENABLE);
			}
		}
		break;
		case BSP_USART6:
		{
			
			bsp_usartTx_enqueue(BSP_USART6,pbuf,len);
			if(bsp_getusartTx_count(BSP_USART6) > 0)
			{
				DMA_Cmd(DMA2_Stream6,DISABLE);	
				bsp_usart_485en_T(BSP_USART6);					
				if(STM32F4xxUSART_Instance[BSP_USART6].TxBuf.In < STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out)
				{
					DMA2_Stream6->NDTR = STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Size - STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out;
					
				}
				else
				{
					DMA2_Stream6->NDTR = STM32F4xxUSART_Instance[BSP_USART6].TxBuf.In - STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out;
				}
				DEBUG("UART SEND LEN :%d\r\n",DMA2_Stream6->NDTR);
				STM32F4xxUSART_Instance[BSP_USART6].TxCount = DMA2_Stream6->NDTR;
				DMA2_Stream6->M0AR = (uint32_t)(STM32F4xxUSART_Instance[BSP_USART6].TxBuf.pData + STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out);	
				DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6);
				DMA_ITConfig(DMA2_Stream6, DMA_IT_TC,  ENABLE);
				DMA_Cmd(DMA2_Stream6,ENABLE);	
			}			
		}
		break;
		default:
		{
			DEBUG("Usart %d is err\r\n",BSP_USARTx);
		}
		break;
	}

	
}

uint16_t BSP_USART_ReadBytes_DMA(uint8_t BSP_USARTx)
{
	return 0;
}


// this send way is very danger !!!!
void BSP_USART_WriteBytes(uint8_t BSP_USARTx,uint8_t* pBuf,uint16_t length)
{
	uint16_t i = 0;
	uint16_t timeout = 0;
	switch (BSP_USARTx)
	{
		case BSP_USART1:
		{
			for(i = 0; i < length; i ++)
			{
				while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET)
				{
					timeout ++ ;
					if(timeout == 1000)
					{
						DEBUG("USART TIME OUT\r\n");
						break;
					}
				}
				USART_SendData( USART1, pBuf[i]);
				
			}
		}
		break;
		case BSP_USART6:
		{
			for(i = 0; i < length; i ++)
			{
				while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) != SET)
				{
					timeout ++ ;
					if(timeout == 1000)
					{
						DEBUG("USART TIME OUT\r\n");
						break;
					}
				}
				USART_SendData( USART6, pBuf[i]);
				
			}		
			
		}
		break;
		default:break;
	}
}
	
uint16_t BSP_USART_ReadBytes(uint8_t BSP_USARTx,uint8_t* pBuf,uint16_t length)
{
	return 0;
}

void BSP_USART1_TxDMA_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) == SET)
	{
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
		
		if(DMA_GetCurrDataCounter(DMA2_Stream7) == 0)
		{
			if(STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Count > STM32F4xxUSART_Instance[BSP_USART1].TxCount)
			{
				STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Count -=STM32F4xxUSART_Instance[BSP_USART1].TxCount;
			}
			else
			{
				STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Count = 0;
			}
			
			STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out += STM32F4xxUSART_Instance[BSP_USART1].TxCount;
			STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out %= STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Size;
			
			if(STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Count > 0)
			{
				DMA_Cmd(DMA2_Stream7,DISABLE);				
				if(STM32F4xxUSART_Instance[BSP_USART1].TxBuf.In < STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out)
				{
					DMA2_Stream7->NDTR = STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Size - STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out;
					
				}
				else
				{
					DMA2_Stream7->NDTR = STM32F4xxUSART_Instance[BSP_USART1].TxBuf.In - STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out;
				}
				STM32F4xxUSART_Instance[BSP_USART1].TxCount = DMA2_Stream7->NDTR;
				DMA2_Stream7->M0AR = (uint32_t)(STM32F4xxUSART_Instance[BSP_USART1].TxBuf.pData + STM32F4xxUSART_Instance[BSP_USART1].TxBuf.Out);
				DMA_Cmd(DMA2_Stream7,ENABLE);	
			}
			else
			{
				USART_ITConfig(USART1, USART_IT_TC, ENABLE);
			}
		}
		
	}
}

void BSP_USART6_TxDMA_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6) == SET)
	{
		DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6);
		
		if(DMA_GetCurrDataCounter(DMA2_Stream6) == 0)
		{
			if(STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Count > STM32F4xxUSART_Instance[BSP_USART6].TxCount)
			{
				STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Count -=STM32F4xxUSART_Instance[BSP_USART6].TxCount;
			}
			else
			{
				STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Count = 0;
			}
			
			STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out += STM32F4xxUSART_Instance[BSP_USART6].TxCount;
			STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out %= STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Size;
			
			if(STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Count > 0)
			{
				DMA_Cmd(DMA2_Stream6,DISABLE);				
				if(STM32F4xxUSART_Instance[BSP_USART6].TxBuf.In < STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out)
				{
					DMA2_Stream6->NDTR = STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Size - STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out;
				}
				else
				{
					DMA2_Stream6->NDTR = STM32F4xxUSART_Instance[BSP_USART6].TxBuf.In - STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out;
				}
				DEBUG("UART SEND LEN :%d\r\n",DMA2_Stream6->NDTR);
				STM32F4xxUSART_Instance[BSP_USART6].TxCount = DMA2_Stream6->NDTR;
				DMA2_Stream6->M0AR = (uint32_t)(STM32F4xxUSART_Instance[BSP_USART6].TxBuf.pData + STM32F4xxUSART_Instance[BSP_USART6].TxBuf.Out);
				DMA_Cmd(DMA2_Stream6,ENABLE);	
			}
			else
			{
				USART_ITConfig(USART6, USART_IT_TC, ENABLE);
			}			
		}
		
		
		
	}
}



void BSP_USART_IRQHandler(uint8_t BSP_USARTx)
{
	switch(BSP_USARTx )
	{
		case BSP_USART1:
		{
			if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
			{
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);
			}
			if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
			{
				
				USART_ClearITPendingBit(USART1,USART_IT_TC);
				if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)
				{
					bsp_usart_485en_R(BSP_USART1);
					USART_ITConfig(USART1, USART_IT_TC, DISABLE);
				}
				
				
			}			
			if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
			{
				USART_ReceiveData(USART1);
				USART_GetITStatus(USART1, USART_IT_IDLE);
				
				//DEBUG("IDEL:%d\r\n",DMA_GetCurrDataCounter(DMA2_Stream5));
				
				STM32F4xxUSART_Instance[BSP_USART1].RxBuf.Count += BSP_USART1_RXBUF_SIZE - DMA2_Stream5->NDTR;
				
				
				
				DMA_Cmd(DMA2_Stream5,DISABLE);
				DMA2_Stream5->M0AR = (uint32_t)USART1_Rx_Buf;
				DMA2_Stream5->NDTR = BSP_USART1_RXBUF_SIZE;
				DMA_Cmd(DMA2_Stream5,ENABLE);
			}
		}
		break;
		case BSP_USART6:
		{
			if(USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
			{
				USART_ClearITPendingBit(USART6,USART_IT_RXNE);
			}
			if(USART_GetITStatus(USART6, USART_IT_TC) == SET)
			{
				
				USART_ClearITPendingBit(USART6,USART_IT_TC);
				if(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == SET)
				{
					bsp_usart_485en_R(BSP_USART6);
					USART_ITConfig(USART6, USART_IT_TC, DISABLE);
				}
				
				
			}
			if(USART_GetITStatus(USART6, USART_IT_IDLE) == SET)
			{
				
				USART_ReceiveData(USART6);
				USART_GetITStatus(USART6, USART_IT_IDLE);
				//DEBUG("IDEL:%d\r\n",DMA_GetCurrDataCounter(DMA2_Stream5));
				DMA_Cmd(DMA2_Stream1,DISABLE);
				App_RevBufToQueue(USART6_Rx_Buf,BSP_USART6_RXBUF_SIZE - DMA2_Stream1->NDTR);
				
				DEBUG("USART 6 REV len :%d\r\n",BSP_USART6_RXBUF_SIZE - DMA2_Stream1->NDTR);
				DMA2_Stream1->M0AR = (uint32_t)USART6_Rx_Buf;
				DMA2_Stream1->NDTR = BSP_USART6_RXBUF_SIZE;
				DMA_Cmd(DMA2_Stream1,ENABLE);
			}			
		}
		break;
		default:break;
	}
	
}
// -------------------Queue -------------------------
void bsp_usartqueue_init(uint8_t BSP_USARTx)
{
	STM32F4xxUSART_Instance[BSP_USARTx].RxBuf.Count = 0;
	STM32F4xxUSART_Instance[BSP_USARTx].RxBuf.In = 0;
	STM32F4xxUSART_Instance[BSP_USARTx].RxBuf.Out = 0;
	STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.Count = 0;
	STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.In = 0;
	STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.Out = 0;
}

static uint16_t bsp_usartTx_getqueuefreespace(uint8_t BSP_USARTx)
{
	return STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.Size - STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.Count;
}

static uint16_t bsp_getusartTx_count(uint8_t BSP_USARTx)
{
	return STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.Count;
}

static int8_t bsp_usartTx_enqueue(uint8_t BSP_USARTx, uint8_t * buf,uint16_t len)
{
	uint8_t * but_ptr = 0;
	but_ptr = buf;
	if(bsp_usartTx_getqueuefreespace(BSP_USARTx) < len)
	{
		DEBUG("usartTx is full");
		return -1;
	}
	while(len --)
	{
		STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.pData[STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.In] = *(but_ptr);
		STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.Count ++;
		STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.In ++;
		but_ptr ++;
		STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.In %= STM32F4xxUSART_Instance[BSP_USARTx].TxBuf.Size;
	}	
	return 1;
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


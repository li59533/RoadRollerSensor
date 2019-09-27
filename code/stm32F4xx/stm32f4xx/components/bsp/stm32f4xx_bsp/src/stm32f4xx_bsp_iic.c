/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_iic.c
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
 * @addtogroup    stm32f4xx_bsp_iic_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_iic_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_iic_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_iic_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_iic_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_iic_Private_Variables 
 * @brief         
 * @{  
 */
#define BSP_IIC2_RXBUFSIZE				200
#define BSP_IIC2_TXBUFSIZE				200
static uint8_t bsp_iic2_Rxbuf[BSP_IIC2_RXBUFSIZE] = { 0 };
static uint8_t bsp_iic2_Txbuf[BSP_IIC2_TXBUFSIZE] = { 0 };

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_iic_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_iic_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_iic_Functions 
 * @brief         
 * @{  
 */
void BSP_IIC_Open(uint8_t BSP_IICx, uint8_t * userparam)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; 
	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	DMA_InitTypeDef DMA_InitStruct = { 0 };	
	I2C_InitTypeDef I2C_InitStruct = { 0 };
	
	switch(BSP_IICx)
	{
		case BSP_IIC1:
		{
			
		}
		break;
		case BSP_IIC2:
		{
			// ---CLK ENABLE----------
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE); //DMA Clock open
			// -----------------------
			// ---GPIO Init-----------
			GPIO_StructInit(&GPIO_InitStruct);		
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_11;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_Init(GPIOB,&GPIO_InitStruct);		

			GPIO_PinAFConfig(GPIOB,GPIO_PinSource10, GPIO_AF_I2C2);			
			GPIO_PinAFConfig(GPIOB,GPIO_PinSource11, GPIO_AF_I2C2);	
			
			// -----------------------
			// --- DMA Init-----------
				// --- Rx ----
			DMA_StructInit(&DMA_InitStruct);
			DMA_InitStruct.DMA_BufferSize = BSP_IIC2_RXBUFSIZE;
			DMA_InitStruct.DMA_Channel = DMA_Channel_7;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
			DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)bsp_iic2_Rxbuf;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
			DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &I2C2->DR;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
			
			DMA_Init( DMA1_Stream2,&DMA_InitStruct);			
			DMA_Cmd(DMA1_Stream2,ENABLE);
				// ---Tx ----
			DMA_StructInit(&DMA_InitStruct);
			DMA_InitStruct.DMA_BufferSize = BSP_IIC2_TXBUFSIZE;
			DMA_InitStruct.DMA_Channel = DMA_Channel_7;
			DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
			DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)bsp_iic2_Txbuf;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
			DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &I2C2->DR;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
			
			DMA_Init( DMA1_Stream7,&DMA_InitStruct);	
			DMA_Cmd(DMA1_Stream7,ENABLE);
			
			// -----------------------
			// ---- IIC Init ---------
			I2C_StructInit(&I2C_InitStruct);
			I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
			I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
			I2C_InitStruct.I2C_ClockSpeed = 400000;
			I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
			I2C_InitStruct.I2C_Mode = I2C_Mode_I2C ;
			I2C_InitStruct.I2C_OwnAddress1 = 0xAA;
			I2C_Init(I2C2, &I2C_InitStruct);
			
			// ---- NVIC Init --------
			// ----------------------------
			I2C_DMALastTransferCmd(I2C2, ENABLE);	
			I2C_DMACmd(I2C2,ENABLE);
			I2C_Cmd(I2C2, ENABLE);		
			
			NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream2_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStruct);
			NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream7_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStruct);
			
		}
		break;
		default:break;
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


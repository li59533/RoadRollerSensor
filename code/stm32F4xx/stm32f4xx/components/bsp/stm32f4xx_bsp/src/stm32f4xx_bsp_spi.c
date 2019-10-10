/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_spi.c
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
#include "ad7988_core.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "clog.h"
/**
 * @addtogroup    stm32f4xx_bsp_spi_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_spi_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_spi_Private_Variables 
 * @brief         
 * @{  
 */
#define ADC_CHANNALS  			1
#define BYTES_PER_CHANNEL		2	
#define BSP_SPI1_TXBUFSIZE	 	ADC_CHANNALS*BYTES_PER_CHANNEL
#define BSP_SPI1_RXBUFSIZE 		ADC_CHANNALS*BYTES_PER_CHANNEL
static uint8_t bsp_spi_Txbuf[BSP_SPI1_TXBUFSIZE] = { 0 };
static uint8_t bsp_spi_Rxbuf[BSP_SPI1_RXBUFSIZE] = { 0 }; 


static uint16_t bsp_spi_RxTemp = 0;


/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_spi_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void bsp_spi1Rx_func(uint16_t *buf);
/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_spi_Functions 
 * @brief         
 * @{  
 */

void BSP_SPI_Open(uint8_t BSP_SPIx, uint8_t *userparams)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; 
	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	DMA_InitTypeDef DMA_InitStruct = { 0 };
	SPI_InitTypeDef SPI_InitStruct = { 0 };
	switch(BSP_SPIx)
	{
		case BSP_SPI1:
		{
			// -------------Clock Init---------------
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
			//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
			// --------------------------------------
			// -------------GPIO Init----------------
			GPIO_StructInit(&GPIO_InitStruct);		
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4|GPIO_Pin_5;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_Init(GPIOB,&GPIO_InitStruct);		
			/* GPIO Alternate functions configuration function ****************************/
			GPIO_PinAFConfig(GPIOB,GPIO_PinSource3, GPIO_AF_SPI1);			
			GPIO_PinAFConfig(GPIOB,GPIO_PinSource4, GPIO_AF_SPI1);
			GPIO_PinAFConfig(GPIOB,GPIO_PinSource5, GPIO_AF_SPI1);
			// --------------------------------------
			// -------------SPI Init ----------------
			SPI_StructInit(&SPI_InitStruct);
			SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
			SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
			SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
			SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
			SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
			SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
			SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
			SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
			
     		SPI_Init(SPI1, &SPI_InitStruct);
			
			NVIC_InitStruct.NVIC_IRQChannel = SPI1_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStruct);
			
			SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
			SPI_Cmd(SPI1, ENABLE);
			// -------------DMA Init-----------------
					// ---- TX----
			/*		
			DMA_StructInit(&DMA_InitStruct);
			DMA_InitStruct.DMA_BufferSize = BSP_SPI1_TXBUFSIZE;
			DMA_InitStruct.DMA_Channel = DMA_Channel_3;
			DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
			DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)bsp_spi_Txbuf;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
			DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &SPI1->DR;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
			DMA_Init( DMA2_Stream3,&DMA_InitStruct);
			SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);			
			
					// ---- RX ----
			DMA_StructInit(&DMA_InitStruct);
			DMA_InitStruct.DMA_BufferSize = BSP_SPI1_RXBUFSIZE;
			DMA_InitStruct.DMA_Channel = DMA_Channel_3;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
			DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)bsp_spi_Rxbuf;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
			DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &SPI1->DR;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
			DMA_Init( DMA2_Stream2,&DMA_InitStruct);
			SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

			DMA_Cmd(DMA2_Stream3, ENABLE);
			DMA_Cmd(DMA2_Stream2, ENABLE);
			// -------- IT conf------------------------
			
			NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream2_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStruct);
			
			DMA_ClearITPendingBit(DMA2_Stream2,DMA_IT_TCIF2);
			DMA_ITConfig(DMA2_Stream2, DMA_IT_TC,  ENABLE);
			*/
			// ----------------------------------------
		}
		break;
		default:break;
	}
}

void BSP_SPI_Start(uint8_t BSP_SPIx)
{
	switch(BSP_SPIx)
	{
		case BSP_SPI1:
		{
			DEBUG("SPI Start\r\n");
			DMA_SetCurrDataCounter(DMA2_Stream2, BSP_SPI1_RXBUFSIZE);//读
			DMA_SetCurrDataCounter(DMA2_Stream3, BSP_SPI1_TXBUFSIZE);//写
			DMA_ClearITPendingBit(DMA2_Stream2,DMA_IT_TCIF2|DMA_IT_FEIF2|DMA_IT_DMEIF2|DMA_IT_TEIF2|DMA_IT_HTIF2);
			DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_TCIF3|DMA_IT_FEIF3|DMA_IT_DMEIF3|DMA_IT_TEIF3|DMA_IT_HTIF3);
			DMA_Cmd(DMA2_Stream2, ENABLE);
			DMA_Cmd(DMA2_Stream3, ENABLE);
		}
		break;
		default:break;
	}

}


void BSP_SPI_Close(uint8_t BSP_SPIx)
{

}
void BSP_SPI_CS_Write(uint8_t BSP_SPIx,uint8_t value)
{
	
}

//  ---- IRQ Handler----------------------
void BSP_SPI1_DMA_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream2,DMA_IT_TCIF2) == SET)
	{
		uint8_t temp = 0;
		temp = bsp_spi_Rxbuf[0];
		bsp_spi_Rxbuf[0] = bsp_spi_Rxbuf[1];
		bsp_spi_Rxbuf[1] = temp;
		bsp_spi1Rx_func((uint16_t *)bsp_spi_Rxbuf);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);	
	}
}

void BSP_SPI1_IRQHandler(void)
{
	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET)
	{
		static uint16_t test_count = 0;
		
		bsp_spi_RxTemp = SPI_I2S_ReceiveData(SPI1);
		//DEBUG("ENTER SPI IRQ %X\r\n",bsp_spi_RxTemp);
		bsp_spi1Rx_func(&bsp_spi_RxTemp);
		SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_RXNE);
	}
}

// ---------------------------------------

static void bsp_spi1Rx_func(uint16_t *buf)
{
	AD7988_CollectOriginalData(buf);
}




uint16_t SPI1_ReadWriteByte(uint16_t TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  

	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  

	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
 		    
}


// --------some func is not come true-------
uint8_t  BSP_SPI_WriteByte(uint8_t BSP_SPIx,uint8_t data);
uint8_t  BSP_SPI_ReadByte(uint8_t BSP_SPIx);
uint16_t BSP_SPI_WriteBytes(uint8_t BSP_SPIx,uint8_t *pBuf,uint16_t length);
uint16_t BSP_SPI_ReadBytes(uint8_t BSP_SPIx,uint8_t *pBuf,uint16_t length);
// -----------------------------------------
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */


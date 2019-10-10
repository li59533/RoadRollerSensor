/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_exti.c
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
 * @addtogroup    stm32f4xx_bsp_exti_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_exti_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_exti_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_exti_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_exti_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_exti_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_exti_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_exti_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_exti_Functions 
 * @brief         
 * @{  
 */
void BSP_EXTI_Open(uint8_t BSP_EXTIx,uint8_t *userparam)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; 
	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	EXTI_InitTypeDef EXTI_InitStruct = { 0 };
	
	switch(BSP_EXTIx)
	{
		case BSP_EXTI_1:
		{
			// ---CLK Open---------------
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT,ENABLE);
			// --------------------------
			// --- GPIO Init-------------
			GPIO_StructInit(&GPIO_InitStruct);		
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_Init(GPIOB,&GPIO_InitStruct);		

			// --- Syscfg exti conf------
			SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource6);
			// --------------------------
			// --- Exti Init-------------
			
			EXTI_StructInit( &EXTI_InitStruct);
			EXTI_InitStruct.EXTI_Line = EXTI_Line6;
			EXTI_InitStruct.EXTI_LineCmd = ENABLE;
			EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
			EXTI_Init(&EXTI_InitStruct);
			// --------------------------
			// -------Init the NVIC-----------------
			NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStruct);
			
			
		}
		break;
		case BSP_EXTI_2:
		{
			
		}
		break;
		default:break;
	}
}

void BSP_EXTI_5_9_Handler(void)
{
	if( EXTI_GetITStatus(EXTI_Line6) == SET)
	{
		uint16_t spi_test = 0;
		//BSP_SPI_Start(BSP_SPI1);
		//spi_test = SPI1_ReadWriteByte(0);
		SPI_I2S_SendData(SPI1, 0);
		EXTI_ClearITPendingBit(EXTI_Line6);
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


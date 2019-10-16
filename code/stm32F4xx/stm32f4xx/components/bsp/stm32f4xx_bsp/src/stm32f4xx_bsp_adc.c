/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_adc.c
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
#include "hal_task.h"
/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    stm32f4xx_bsp_adc_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_Macros_Defines 
 * @brief         
 * @{  
 */
#define BSP_ADC_REGISTER_BUFSIZE			200
/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_adc_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_adc_Private_Variables 
 * @brief         
 * @{  
 */

	
static BSP_ADC_Value_t bsp_adc_value[BSP_ADC_COUNT];


static uint16_t bsp_adc_register_buf[BSP_ADC_REGISTER_BUFSIZE][BSP_ADC_COUNT] = { 0 };
/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_adc_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_Functions 
 * @brief         
 * @{  
 */
 
// ------PB0 ADC1_IN8 
int8_t BSP_ADC_Open(uint8_t BSP_ADCx, uint8_t *userparams)
{
	ADC_InitTypeDef ADC_InitStruct = { 0 };
	GPIO_InitTypeDef GPIO_InitStruct = { 0 }; 
	DMA_InitTypeDef DMA_InitStruct = { 0 };
	ADC_CommonInitTypeDef ADC_CommonInitStruct = { 0 };
	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	switch(BSP_ADCx)
	{
		case BSP_ADC1:
		{
			// --------Clock Open--------------
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
			// --------GPIO Init---------------
			GPIO_StructInit(&GPIO_InitStruct);		
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
			GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_Init(GPIOB,&GPIO_InitStruct);		
			// --------DMA Channel Init--------			
			DMA_StructInit(&DMA_InitStruct);
			DMA_InitStruct.DMA_BufferSize = BSP_ADC_REGISTER_BUFSIZE * BSP_ADC_COUNT;
			DMA_InitStruct.DMA_Channel = DMA_Channel_0;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
			DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)bsp_adc_register_buf;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
			
			DMA_Init( DMA2_Stream0,&DMA_InitStruct);

			DMA_Cmd(DMA2_Stream0,ENABLE);

			// --------ADC CommonInit----------
			ADC_CommonStructInit( &ADC_CommonInitStruct);
			ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
			ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
			ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4 ;
			ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
			
			ADC_CommonInit(&ADC_CommonInitStruct);
			
			// --------ADC init----------------			
			ADC_StructInit(&ADC_InitStruct);
			ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
			ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
			ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
			ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
			ADC_InitStruct.ADC_NbrOfConversion = BSP_ADC_COUNT;
			ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
			ADC_InitStruct.ADC_ScanConvMode = ENABLE;
			
			ADC_Init( ADC1 , &ADC_InitStruct);
			
			// ---------------------------------	
			// -------Init the NVIC-------------
			NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream0_IRQn;
			NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStruct);
			
			DMA_ITConfig(DMA2_Stream0, DMA_IT_TC,  ENABLE);
			
			// ----------- OPEN ----------------
			ADC_ContinuousModeCmd(ADC1, ENABLE);
			ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_144Cycles);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_144Cycles);
			ADC_DMACmd(ADC1, ENABLE);
			ADC_Cmd( ADC1, DISABLE);
		}
		break;
		default:break;
	}
	
	return 1;
}



int8_t BSP_ADC_Close(uint8_t BSP_ADCx)
{
	return 0;
}
void BSP_ADC_Start(uint8_t BSP_ADCx)
{
	DMA_Cmd(DMA2_Stream0, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
}
void BSP_ADC_Stop(uint8_t BSP_ADCx)
{
	ADC_Cmd(ADC1, DISABLE);
	ADC_DMACmd(ADC1, DISABLE);
	DMA_Cmd(DMA2_Stream0, DISABLE);
	DMA2_Stream0->M0AR = (uint32_t)&bsp_adc_register_buf;
	DMA2_Stream0->NDTR = BSP_ADC_REGISTER_BUFSIZE * BSP_ADC_COUNT;
}

uint16_t BSP_ADC_GetRegisterValue(uint8_t BSP_ADCx)
{
	return bsp_adc_value[BSP_ADCx].RegisterValue;
}

uint16_t BSP_ADC_GetVolmvValue(uint8_t BSP_ADCx)
{
	return bsp_adc_value[BSP_ADCx].VolmvValue;
}

void BSP_ADC1_DMA_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET)
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		BSP_ADC_Stop( 0 );
		HalTask_Send_Event(HAL_TASK_ADC_CALC_EVENT);
	}
}

void BSP_ADC1_Calc_Process(void)
{
	uint16_t i = 0;
	uint32_t sum[BSP_ADC_COUNT] = {0};
	for(i = 0; i < BSP_ADC_REGISTER_BUFSIZE;i ++)
	{
		sum[BSP_ADC1] += bsp_adc_register_buf[i][BSP_ADC1];
		sum[BSP_ADC2] += bsp_adc_register_buf[i][BSP_ADC2];
	}
	
	bsp_adc_value[BSP_ADC1].RegisterValue = (uint16_t)(sum[BSP_ADC1] / BSP_ADC_REGISTER_BUFSIZE);
	bsp_adc_value[BSP_ADC2].RegisterValue = (uint16_t)(sum[BSP_ADC2] / BSP_ADC_REGISTER_BUFSIZE);
	bsp_adc_value[BSP_ADC1].VolmvValue = (uint16_t )(bsp_adc_value[BSP_ADC1].RegisterValue * 0.8056f);
	bsp_adc_value[BSP_ADC2].VolmvValue = (uint16_t )(bsp_adc_value[BSP_ADC2].RegisterValue * 0.8056f);
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


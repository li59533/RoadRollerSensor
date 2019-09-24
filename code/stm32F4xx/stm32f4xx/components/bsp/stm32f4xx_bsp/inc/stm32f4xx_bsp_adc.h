/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_adc.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _STM32F4XX_BSP_ADC_H_
#define _STM32F4XX_BSP_ADC_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    stm32f4xx_bsp_adc_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_Exported_Macros 
 * @{  
 */
#define BSP_ADC1		0
#define BSP_ADC2		1

#define BSP_ADC_COUNT 	1
/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_adc_Exported_Types 
 * @{  
 */
typedef struct 
{
	uint16_t RegisterValue;
	uint16_t VolmvValue;
}BSP_ADC_Value_t;
/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_adc_Exported_Functions 
 * @{  
 */
int8_t BSP_ADC_Open(uint8_t BSP_ADCx, uint8_t *userparams);
int8_t BSP_ADC_Close(uint8_t BSP_ADCx);
void BSP_ADC_Start(uint8_t BSP_ADCx);
void BSP_ADC_Stop(uint8_t BSP_ADCx);
uint16_t BSP_ADC_GetRegisterValue(uint8_t BSP_ADCx);
uint16_t BSP_ADC_GetVolmvValue(uint8_t BSP_ADCx);

void BSP_ADC1_DMA_IRQHandler(void);

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


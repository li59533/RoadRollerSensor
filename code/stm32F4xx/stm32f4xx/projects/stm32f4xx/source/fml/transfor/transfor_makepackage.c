/**
 **************************************************************************************************
 * @file        transfor_makepackage.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "transfor_makepackage.h"
#include "stm32_bsp_conf.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "lnprotocol.h"
#include "bsp_led.h"
/**
 * @addtogroup    transfor_makepackage_Modules 
 * @{  
 */

/**
 * @defgroup      transfor_makepackage_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      transfor_makepackage_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      transfor_makepackage_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      transfor_makepackage_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      transfor_makepackage_Private_Variables 
 * @brief         
 * @{  
 */
#define TRANSFOR_PACKAGE_SPACE_LEN		200 
static uint8_t transfor_package_space[TRANSFOR_PACKAGE_SPACE_LEN] = { 0 };

/**
 * @}
 */
 
/**
 * @defgroup      transfor_makepackage_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      transfor_makepackage_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      transfor_makepackage_Functions 
 * @brief         
 * @{  
 */

uint8_t * Transfor_MakePackage(uint16_t * len, Trans485_datavalue_t * data)
{
	uint8_t i = 0;
	
	Trans485_dataform_t * buf_ptr = (Trans485_dataform_t *)transfor_package_space;
	buf_ptr->head = LNPROTOCOL_HEAD;
	buf_ptr->cmd = TAG_UPDATA;
	buf_ptr->length = sizeof(Trans485_datavalue_t) ;
	buf_ptr->acc_pfak = data->acc_pfak;
	buf_ptr->base_frequency = data->base_frequency;
	buf_ptr->harmonic_peak_0_5 = data->harmonic_peak_0_5;
	buf_ptr->harmonic_peak_1 = data->harmonic_peak_1;
	buf_ptr->harmonic_peak_1_5 = data->harmonic_peak_1_5;
	buf_ptr->harmonic_peak_2 = data->harmonic_peak_2;
	buf_ptr->harmonic_peak_2_5 = data->harmonic_peak_2_5;
	buf_ptr->harmonic_peak_3 = data->harmonic_peak_3;
	buf_ptr->harmonic_peak_4 = data->harmonic_peak_4;
	buf_ptr->harmonic_peak_5 = data->harmonic_peak_5;
	buf_ptr->harmonic_peak_6 = data->harmonic_peak_6;
	buf_ptr->speed_peak = data->speed_peak;
	buf_ptr->offset_peak = data->offset_peak;
	buf_ptr->temperature = data->temperature;
	buf_ptr->power = data->power;
	
	buf_ptr->sum_check = 0;
	for(i = 0; i <( &buf_ptr->sum_check - &buf_ptr->head); i ++)
	{
		buf_ptr->sum_check += *((uint8_t *)buf_ptr + i);
	}
	buf_ptr->foot = LNPROTOCOL_FOOT;
	
	*len = sizeof(Trans485_dataform_t);
	return transfor_package_space;
	
}



void Transfor_Send(uint8_t * buf, uint16_t len)
{
	BSP_LED_Blink(BSP_LED_DATA, 3, 10, 100);
	BSP_USART_WriteBytes_DMA( BSP_USART1,buf, len);
	BSP_USART_WriteBytes_DMA( BSP_USART6,buf, len);
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


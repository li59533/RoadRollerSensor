/**
 **************************************************************************************************
 * @file        ad7988_core.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "ad7988_core.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "malloc.h"
#include "vibrate_task.h"
#include "stm32_bsp_conf.h"
#include "bsp_led.h"
#include "clog.h"
#include "arm_math.h"
#include "arm_const_structs.h"
/**
 * @addtogroup    ad7988_core_Modules 
 * @{  
 */

/**
 * @defgroup      ad7988_core_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_core_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      ad7988_core_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_core_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      ad7988_core_Private_Variables 
 * @brief         
 * @{  
 */

static uint16_t * ad7988_fftbuf[2];
static uint8_t ad7988_status = 0;


/**
 * @}
 */
 
/**
 * @defgroup      ad7988_core_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_core_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_core_Functions 
 * @brief         
 * @{  
 */
 
void AD7988_ParamInit(void)
{
	ad7988_fftbuf[0]= (uint16_t *)mymalloc(SRAMCCM,AD7988_FFT_LENGTH*sizeof(uint16_t) );
	ad7988_fftbuf[1]= (uint16_t *)mymalloc(SRAMCCM,AD7988_FFT_LENGTH*sizeof(uint16_t) );
}	
 
void AD7988_CollectOriginalData(uint16_t *buf) // this add fftbuf, when the fftbuf is full ,will start the calc event
{
	static uint16_t ad_position = 0;
	static uint8_t current_read_buffer = 0;
	
	ad7988_fftbuf[current_read_buffer][ad_position ++] = *buf;
	if(ad_position >= AD7988_SAMPLE_RATE)
	{
		ad_position = 0;
		current_read_buffer = !current_read_buffer;
		VibrateTask_Send_Event(VIBRATE_TASK_CALC_EVENT);// this place to triger some event
	}
}

void AD7988_Calc_Process(void)  
{
	
}

void AD7988_Status_CheckProcess(void)
{
	uint16_t status_advalue = 0;
	status_advalue = BSP_ADC_GetVolmvValue(BSP_ADC2);
	
	DEBUG("AD7988 status_advalue:%d\r\n",status_advalue);
	
	if(status_advalue >=  4000)
	{
		ad7988_status = AD7988_Err1;
		BSP_LED_Close(BSP_LED_VIBRATOR);
	}
	else if(status_advalue <= 50)
	{
		ad7988_status = AD7988_Err2;
		BSP_LED_Blink(BSP_LED_VIBRATOR, 0, 10,200);
	}
	else
	{
		ad7988_status = AD7988_Normal;
		BSP_LED_Open(BSP_LED_VIBRATOR);
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


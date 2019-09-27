/**
 **************************************************************************************************
 * @file        t_4_20mv.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "t_4_20mv.h"

/**
 * @addtogroup    XXX 
 * @{  
 */
#include "stm32_bsp_conf.h"
#include "bsp_led.h"
#include "clog.h"
/**
 * @addtogroup    t_4_20mv_Modules 
 * @{  
 */

/**
 * @defgroup      t_4_20mv_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      t_4_20mv_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      t_4_20mv_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      t_4_20mv_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      t_4_20mv_Private_Variables 
 * @brief         
 * @{  
 */
 
typedef struct
{
	uint16_t adcmv;
	float tvalue;
	uint8_t status;
}T_4_20mvValue_t;	
 
static T_4_20mvValue_t t_4_20mvValue = { 0 };
/**
 * @}
 */
 
/**
 * @defgroup      t_4_20mv_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      t_4_20mv_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      t_4_20mv_Functions 
 * @brief         
 * @{  
 */
void T_4_20mv_CalcProcess(void)
{
	t_4_20mvValue.adcmv = BSP_ADC_GetVolmvValue(BSP_ADC1);
	
	DEBUG("T_420mV Calc:%d\r\n",t_4_20mvValue.adcmv);
	
	if(t_4_20mvValue.adcmv >= 600 && t_4_20mvValue.adcmv <= 3000 )
	{
		BSP_LED_Open(BSP_LED_TEMPERATURE);
	}
	else
	{
		BSP_LED_Close(BSP_LED_TEMPERATURE);
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


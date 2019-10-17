/**
 **************************************************************************************************
 * @file        ad7988_core.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _AD7988_CORE_H_
#define _AD7988_CORE_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    ad7988_core_Modules 
 * @{  
 */

/**
 * @defgroup      ad7988_core_Exported_Macros 
 * @{  
 */
// ------AD7988-1 USER CONF PARAM--- 
#define AD7988_SAMPLE_RATE	4096
#define AD7988_SAMPLE_LEN	4096
#define AD7988_GRAVITY		1000 //9810.5f
#define AD7988_FRQ_MIN		5
#define AD7988_FRQ_MAX		200
// ---------------------------------
#define AD7988_FFT_LENGTH	AD7988_SAMPLE_RATE    //FFT length

typedef enum
{
	AD7988_Normal = 0,
	AD7988_Err1	  = 1,
	AD7988_Err2	  = 2,
}AD7988_Status_e;

/**
 * @}
 */

/**
 * @defgroup      ad7988_core_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      ad7988_core_Exported_Types 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_core_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      ad7988_core_Exported_Functions 
 * @{  
 */

void AD7988_ParamInit(void);
void AD7988_CollectOriginalData(uint16_t *buf);
void AD7988_Calc_Process(void)  ;
void AD7988_Status_CheckProcess(void);

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

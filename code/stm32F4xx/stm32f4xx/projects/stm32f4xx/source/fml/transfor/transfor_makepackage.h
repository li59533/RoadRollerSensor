/**
 **************************************************************************************************
 * @file        transfor_makepackage.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _TRANSFOR_MAKEPACKAGE_H_
#define _TRANSFOR_MAKEPACKAGE_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    transfor_makepackage_Modules 
 * @{  
 */
#include "self_def.h"
/**
 * @defgroup      transfor_makepackage_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      transfor_makepackage_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      transfor_makepackage_Exported_Types 
 * @{  
 */
	
#pragma pack(1)   
typedef struct
{
	uint8_t head;
	uint8_t cmd;
	uint16_t length;
	float acc_peak;
	float base_frequency;
	float harmonic_peak_0_5;
	float harmonic_peak_1;
	float harmonic_peak_1_5;
	float harmonic_peak_2;
	float harmonic_peak_2_5;
	float harmonic_peak_3;
	float harmonic_peak_4;
	float harmonic_peak_5;
	float harmonic_peak_6;
	float speed_peak;
	float offset_peak;
	float temperature;
	uint8_t  power;
	float acc_mean;
	float acc_min;
	float displace_min;	
	uint8_t sum_check;
	uint8_t foot;
}Trans485_dataform_t;

  


typedef struct
{
	float acc_peak;
	float base_frequency;
	float harmonic_peak_0_5;
	float harmonic_peak_1;
	float harmonic_peak_1_5;
	float harmonic_peak_2;
	float harmonic_peak_2_5;
	float harmonic_peak_3;
	float harmonic_peak_4;
	float harmonic_peak_5;
	float harmonic_peak_6;
	float speed_peak;
	float displace_peak;
	float temperature;
	uint8_t  power;
	float acc_mean;
	float acc_min;
	float displace_min;
}Trans485_datavalue_t;

#pragma pack() 
/**
 * @}
 */

/**
 * @defgroup      transfor_makepackage_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      transfor_makepackage_Exported_Functions 
 * @{  
 */

uint8_t * Transfor_MakePackage(uint16_t * len, Trans485_datavalue_t * data);
void Transfor_Send(uint8_t * buf, uint16_t len);

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

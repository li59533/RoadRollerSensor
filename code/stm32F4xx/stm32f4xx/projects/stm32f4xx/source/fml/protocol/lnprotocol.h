/**
 **************************************************************************************************
 * @file        lnprotocol.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _LNPROTOCOL_H_
#define _LNPROTOCOL_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    lnprotocol_Modules 
 * @{  
 */

/**
 * @defgroup      lnprotocol_Exported_Macros 
 * @{  
 */
#define LNPROTOCOL_HEAD			0X7E

#define LNPROTOCOL_FOOT			0X7E


/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      lnprotocol_Exported_Types 
 * @{  
 */
typedef enum 
{
	TAG_UPDATA	= 0x42,
	TAG_7988MVTOACC_P = 0x44,
}LN_Protocol_Tag_e;

typedef enum
{
	CONF_SUCCESS = 1,
	CONF_ERROR	 = 2,
}LN_Conf_Status_e;


typedef enum
{
	CMD_CONF = 0x01,
	
}LN_Protocol_Cmd_e;



#pragma pack(1)
typedef struct
{
	uint8_t Tag;
	uint8_t Len;
	union 
	{
		uint8_t BIT_8;
		uint16_t BIT_16;
		uint32_t BIT_32;
		uint8_t Array[20];
	}Value;
}LN_Tlv_t;
#pragma pack()

/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      lnprotocol_Exported_Functions 
 * @{  
 */
uint8_t * LNprotocol_AddHead(uint8_t * targetspace_ptr); // return the value ptr

uint8_t * LNprotocol_AddValue(uint8_t * targetspace_ptr , uint8_t *buf,uint8_t len);

uint8_t * LNprotocol_AddFoot(uint8_t * targetspace_ptr);

void LNprotocol_Addlen(uint8_t *buf, uint16_t len);

uint8_t LNprotocol_AddChecksum(uint8_t * buf,uint16_t len);

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

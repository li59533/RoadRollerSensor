/**
 **************************************************************************************************
 * @file        app_conf.h
 * @author
 * @version    v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#ifndef _APP_CONF_H_
#define _APP_CONF_H_

/**
 * @addtogroup    XXX 
 * @{ 
 */
#include "self_def.h"
/**
 * @addtogroup    app_conf_Modules 
 * @{  
 */

/**
 * @defgroup      app_conf_Exported_Macros 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Exported_Constants
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      app_conf_Exported_Types 
 * @{  
 */
typedef struct
{
	uint8_t buf[100];
	uint8_t len;
}App_confbuf_t;
 
typedef struct 
{
	App_confbuf_t  value[3];
	uint8_t in;
	uint8_t out;
	uint8_t count;
	uint8_t size;
}App_ConfQueue_t;
/**
 * @}
 */

/**
 * @defgroup      app_conf_Exported_Variables 
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Exported_Functions 
 * @{  
 */
 
void App_RevBufToQueue(uint8_t *buf,uint16_t len);
void App_RevProcess(void);
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

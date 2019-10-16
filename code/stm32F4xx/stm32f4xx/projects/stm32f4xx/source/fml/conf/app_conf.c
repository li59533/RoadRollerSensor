/**
 **************************************************************************************************
 * @file        app_conf.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "app_conf.h"
#include "user_task.h"
#include "clog.h"
#include "lnprotocol.h"
/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    app_conf_Modules 
 * @{  
 */

/**
 * @defgroup      app_conf_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      app_conf_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Private_Types
 * @brief         
 * @{  
 */
  
#pragma pack(1)
typedef struct
{
	uint8_t head;
	uint16_t len;
	uint8_t cmd;
	uint8_t *payload;
	uint8_t foot;
	uint8_t sum;
}ln_protocolintance_t;
#pragma pack()


/**
 * @}
 */
 
/**
 * @defgroup      app_conf_Private_Variables 
 * @brief         
 * @{  
 */
App_ConfQueue_t App_ConfQueue = {
	.in = 0,
	.out = 0,
	.count = 0,
	.size = 3,
};
/**
 * @}
 */
 
/**
 * @defgroup      app_conf_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      app_conf_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void app_lnprotocol_process(uint8_t *buf,uint16_t len);
static void app_conf_process(uint8_t *payload,uint16_t len);
static void app_lnswitch_cmd(uint8_t * buf,uint16_t len);
/**
 * @}
 */

/**
 * @defgroup      app_conf_Functions 
 * @brief         
 * @{  
 */



void App_RevBufToQueue(uint8_t *buf,uint16_t len)
{
	memcpy(App_ConfQueue.value[App_ConfQueue.in].buf,buf,len);
	App_ConfQueue.value[App_ConfQueue.in].len = len;
	App_ConfQueue.count ++;
	App_ConfQueue.in ++;
	App_ConfQueue.in %= App_ConfQueue.size;
	
	UserTask_Send_Event(USER_TASK_CONF_EVENT);
}

void App_RevProcess(void)
{
	if(App_ConfQueue.count > 0)
	{
		app_lnprotocol_process(App_ConfQueue.value[App_ConfQueue.out].buf,App_ConfQueue.value[App_ConfQueue.out].len  );
		App_ConfQueue.out ++;
		App_ConfQueue.out %= App_ConfQueue.size;
		App_ConfQueue.count -- ;
	}
	else
	{
		DEBUG("App_Rev None\r\n");
	}
}
// head len cmd tlv foot sum

static void app_lnprotocol_process(uint8_t *buf,uint16_t len)
{
	ln_protocolintance_t *ln_protocolintance = (ln_protocolintance_t *)buf;
	if( ln_protocolintance->head == LNPROTOCOL_HEAD)
	{
		if(ln_protocolintance->len == len)
		{
			if(buf[len - 2] == LNPROTOCOL_FOOT)
			{
				if(LNprotocol_AddChecksum(buf,len - 1) == buf[len -1])
				{
					app_lnswitch_cmd(&ln_protocolintance->cmd,len - sizeof(ln_protocolintance_t) + 1);
				}
			}
		}
	}
}

static void app_lnswitch_cmd(uint8_t * buf,uint16_t len)
{
	uint8_t cmd = 0;
	cmd = buf[0];
	switch(cmd)
	{
		case CMD_CONF:app_conf_process(buf + 1, len -1 );break;
		default:break;
	}
}

static void app_conf_process(uint8_t *payload,uint16_t len)
{
	uint8_t * payload_ptr = 0;
	
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


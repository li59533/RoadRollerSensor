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
#include "system_param.h"
#include "transfor_makepackage.h"
#include "version.h"
#include "ad7988.h"

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
#define APP_CONF_DATASPACE_LEN 		500
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

static uint8_t  app_conf_dataspace[APP_CONF_DATASPACE_LEN] = { 0 };
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
static void app_getconfreq_process(uint8_t *payload,uint16_t len);
static void app_setconfreq_process(uint8_t *payload,uint16_t len);
static void app_lnswitch_cmd(uint8_t * buf,uint16_t len);
static void app_getversionreq_process(uint8_t *payload,uint16_t len);
static void app_getvalue_process(uint8_t * payload , uint16_t len);
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
		DEBUG("App_Rev Something len : %d\r\n",App_ConfQueue.value[App_ConfQueue.out].len  );
		app_lnprotocol_process(App_ConfQueue.value[App_ConfQueue.out].buf , App_ConfQueue.value[App_ConfQueue.out].len  );
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
/*

*/

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
					app_lnswitch_cmd(&ln_protocolintance->cmd,len - sizeof(ln_protocolintance) - 1);
				}
				else
				{
					DEBUG("APP_Rev Check sum is err");
				}
			}
			else
			{
				DEBUG("APP_Rev Foot is err\r\n");
			}
		}
		else
		{
			DEBUG("APP_Rev Len is err\r\n");
		}
	}
	else
	{
		DEBUG("APP_Rev Head is err\r\n");
	}
}

static void app_lnswitch_cmd(uint8_t * buf,uint16_t len)
{
	uint8_t cmd = 0;
	cmd = buf[0];
	DEBUG("APP_REV CMD is %X\r\n",cmd);
	
	switch(cmd)
	{
		case CMD_SetConf_Req:app_setconfreq_process(buf + 1, len -1 ) ; break;
		case CMD_GetConf_Req:app_getconfreq_process(buf + 1, len -1 ) ; break;
		case CMD_GetVersion_Req: app_getversionreq_process(buf + 1, len -1 ) ; break;
		case CMD_GetValue_Req: app_getvalue_process(buf + 1, len -1) ; break;
		default:break;
	}
}



static void app_getvalue_process(uint8_t * payload , uint16_t len)
{
	uint8_t * buf_ptr = 0;
	buf_ptr = Transfor_MakePackage(&len,AD7988_GetValue());
	Transfor_Send(buf_ptr,len);	
}

static void app_setconfreq_process(uint8_t *payload,uint16_t len)
{

	uint8_t * payload_ptr = 0;
	LN_Tlv_t * tlv_buf = 0;
	uint8_t result = CONF_SUCCESS;
	
	
	payload_ptr = payload;
	DEBUG("APP_Rev Payload LEN is %d\r\n",len);
	while(1)
	{
		tlv_buf = (LN_Tlv_t *) payload_ptr;
		DEBUG("APP_Rev Tag is %X\r\n",tlv_buf->Tag);
		switch(tlv_buf->Tag)
		{
			case TAG_7988MVTOACC_P:
			{
				float float_temp = 0.0f;

				memcpy(&float_temp,tlv_buf->Value.Array ,tlv_buf->Len);
				// ---------DEBUG-------------------
				char test_buf[30];
				snprintf(test_buf,30,"%f",float_temp);
				DEBUG("float_temp:%s\r\n",test_buf);
				// ---------------------------------
				
				if((float_temp <= 1000.0f)&&(float_temp > 0.0f))
				{
					memcpy(&g_SystemParam_Config.AD7988_VolACC_p,tlv_buf->Value.Array,tlv_buf->Len);
				}
				else
				{
					result = CONF_ERROR;
					DEBUG("APP_Conf AD7988_VolACC_p is False\r\n");
				}
				
			}
			break;
			case TAG_AUTO_REPORT_FALG:
			{
				if(tlv_buf->Value.BIT_8 == 1 || tlv_buf->Value.BIT_8 == 0)
				{
					g_SystemParam_Config.Auto_Report_Flag = tlv_buf->Value.BIT_8;
				}
				else
				{
					result = CONF_ERROR;
					DEBUG("TAG_AUTO_REPORT_FALG is False\r\n");
				}
			}
			break;
			case TAG_INTEG_FREQ_MIN:
			{
				g_SystemParam_Config.integ_freq_min = tlv_buf->Value.BIT_8;
			}
			break;
			case TAG_INTEG_FREQ_MAX:
			{
				g_SystemParam_Config.integ_freq_max = tlv_buf->Value.BIT_8;
			}
			break;			
			default:
			{
				result = CONF_ERROR;
				DEBUG("APP_Conf Tag is miss\r\n");
			}
			break;
		}
		
		payload_ptr = payload_ptr + tlv_buf->Len + 2 ;
		
		if((payload_ptr - payload)>= (len - 2))
		{
			break;
		}
	}

	if(result == CONF_ERROR)
	{
		DEBUG("APP_Conf is Err\r\n");
	}
	else
	{
		DEBUG("APP_Conf is SUCCESS then save\r\n");
		SystemParam_Save();
	}
	
}

// head len cmd tlv foot sum

static void app_getconfreq_process(uint8_t *payload,uint16_t len)
{
	ln_protocolintance_t *ln_protocolintance = (ln_protocolintance_t * )app_conf_dataspace;
	LN_Tlv_t tlv_value = { 0 };
	uint8_t  * payload_ptr = 0;
	float float_temp = 0.0;
	
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	//ln_protocolintance->len = sizeof(ln_protocolintance_t);
	ln_protocolintance->inf = 0x00;
	ln_protocolintance->cmd = CMD_GetConf_Resp;
	
	payload_ptr = &ln_protocolintance->cmd + 1;
	// ------- mvToAcc_p---------------
	tlv_value.Tag = TAG_7988MVTOACC_P;
	tlv_value.Len = 4;
	float_temp = g_SystemParam_Config.AD7988_VolACC_p;
	memcpy(tlv_value.Value.Array, &float_temp , 4);
	payload_ptr += LN_AddTlv(payload_ptr, &tlv_value);
	// --------------------------------
	// ------- AutoReport Flag---------------
	tlv_value.Tag = TAG_AUTO_REPORT_FALG;
	tlv_value.Len = 1;
	tlv_value.Value.BIT_8 = g_SystemParam_Config.Auto_Report_Flag;
	payload_ptr += LN_AddTlv(payload_ptr, &tlv_value);
	// ------- TAG_INTEG_FREQ_MIN---------------
	tlv_value.Tag = TAG_INTEG_FREQ_MIN;
	tlv_value.Len = 1;
	tlv_value.Value.BIT_8 = g_SystemParam_Config.integ_freq_min;
	payload_ptr += LN_AddTlv(payload_ptr, &tlv_value);	
	// --------------------------------
	// ------- TAG_INTEG_FREQ_MAX---------------
	tlv_value.Tag = TAG_INTEG_FREQ_MAX;
	tlv_value.Len = 1;
	tlv_value.Value.BIT_8 = g_SystemParam_Config.integ_freq_max;
	payload_ptr += LN_AddTlv(payload_ptr, &tlv_value);	
	// --------------------------------
	
	ln_protocolintance->len = (payload_ptr - &ln_protocolintance->head) + 1;
	*payload_ptr = LNPROTOCOL_FOOT;
	*(payload_ptr + 1) =  LNprotocol_AddChecksum((uint8_t * )&ln_protocolintance,ln_protocolintance->len - 1);
	Transfor_Send(app_conf_dataspace,ln_protocolintance->len);
	
}

static void app_getversionreq_process(uint8_t *payload,uint16_t len)
{
	ln_protocolintance_t *ln_protocolintance = (ln_protocolintance_t * )app_conf_dataspace;
	uint8_t  * payload_ptr = 0;
	uint32_t version = 0 ;
	
	version = Version_Get_Bin();
	
	ln_protocolintance->head = LNPROTOCOL_HEAD;
	//ln_protocolintance->len = sizeof(ln_protocolintance_t);
	ln_protocolintance->inf = 0x00;
	ln_protocolintance->cmd = CMD_GetVersion_Resp;
	
	payload_ptr = &ln_protocolintance->cmd + 1;
	// -------version---------------
	memcpy(payload_ptr,&version,4);
	payload_ptr += 4;
	// --------------------------------
	ln_protocolintance->len = (payload_ptr - &ln_protocolintance->head) + 1;
	*payload_ptr = LNPROTOCOL_FOOT;
	*(payload_ptr + 1) =  LNprotocol_AddChecksum((uint8_t * )&ln_protocolintance,ln_protocolintance->len - 1);
	
	Transfor_Send(app_conf_dataspace,ln_protocolintance->len);
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


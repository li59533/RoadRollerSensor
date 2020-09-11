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
#include "transfor_makepackage.h"
#include "t_4_20mv.h"
#include "bsp_fft_integral.h"
#include "system_param.h"

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
static uint8_t ad7988_status = 0;
static uint8_t ad7988_valueIndex = 0;

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
//static float ad7988_intrgralValue_space[AD7988_SAMPLE_LEN] = { 0 };
static uint16_t ad7988_fftbuf[2][AD7988_FFT_LENGTH] = { 0 };
static float ad7988_float_accbuf[AD7988_SAMPLE_LEN];
static uint8_t ad7688_fftbuf_sw = 0;
static uint8_t ad7688_fftbuf_calc = 0;

void AD7988_ParamInit(void)
{
	//ad7988_fft_inputbuf = (float *)mymalloc(SRAMCCM,AD7988_FFT_LENGTH*sizeof(float) );
	//ad7988_fftbuf[0]= (uint16_t *)mymalloc(SRAMCCM,AD7988_FFT_LENGTH*sizeof(uint16_t) );
	//ad7988_fftbuf[1]= (uint16_t *)mymalloc(SRAMCCM,AD7988_FFT_LENGTH*sizeof(uint16_t) );
	
	//arm_cfft_radix2_init_f32(&ad7988_scfft,AD7988_FFT_LENGTH,0,1);//
	
}	
 
void AD7988_CollectOriginalData(uint16_t *buf) // this add fftbuf, when the fftbuf is full ,will start the calc event
{
	static uint16_t ad_position = 0;
	static uint8_t current_read_buffer = 0;
	
	ad7988_fftbuf[ad7688_fftbuf_sw][ad_position ++] = *buf;
	if(ad_position >= AD7988_SAMPLE_RATE)
	{
		ad7688_fftbuf_sw = !ad7688_fftbuf_sw;
		ad7688_fftbuf_calc = !ad7688_fftbuf_sw;
		// ----------Stop Sample----------------------
		//BSP_Tim_Stop(BSP_TIM1);
		ad_position = 0;
		VibrateTask_Send_Event(VIBRATE_TASK_CALC_EVENT);// this place to triger some event
	}
}

static void ad7988_clear_originalspace(void)
{
	
}

Trans485_datavalue_t trans485data = { 0 };

void AD7988_Calc_Process(void)  
{
	
	uint16_t i = 0;
	uint8_t * buf_ptr = 0;
	uint16_t len = 0;
	fft_instance_t fft_instance = { 0 };
	uint16_t *pread;
	int16_t * real_signal;
	uint32_t ad7988_sum = 0;
	uint16_t ad7988_average = 0;
	float mvtoacc_p = 1.0f;
	
	
	pread = ad7988_fftbuf[ad7688_fftbuf_calc];

	if((g_SystemParam_Config.AD7988_VolACC_p <= 1000.0f)&&(g_SystemParam_Config.AD7988_VolACC_p > 0.0f))
	{
		mvtoacc_p = (float)(1.0f / g_SystemParam_Config.AD7988_VolACC_p);
	}
	//mvtoacc_p = 0.02f;
	// -------------------------------------------
	// ---------- remove 2.5V ref  , get real acc value -------------------
	for( i = 0; i < AD7988_SAMPLE_LEN ; i ++)
	{
		ad7988_sum += pread[i];
	}
	ad7988_average = (uint16_t )(ad7988_sum / AD7988_SAMPLE_LEN);
	
	real_signal = (int16_t *)pread ;
	for( i = 0 ; i < AD7988_SAMPLE_LEN ; i++)
	{
		real_signal[i] = pread[i] - ad7988_average;
	}
	for(i = 0; i < AD7988_SAMPLE_LEN ; i ++)
	{
		ad7988_float_accbuf[i] = (float)(real_signal[i] * 0.0763f * mvtoacc_p)   *   (0.5f - 0.5f * arm_cos_f32(2.0f * 3.14159265f * i / AD7988_SAMPLE_LEN));//the real acc value  add hanni windows
	}
	
	
	// ---------- fft calc------------------------

	//BSP_FFT_IntegInit(AD7988_SAMPLE_LEN,AD7988_SAMPLE_RATE,AD7988_GRAVITY,AD7988_FRQ_MIN,AD7988_FRQ_MAX); // integral conf	
	BSP_FFT_IntegInit(AD7988_SAMPLE_LEN,AD7988_SAMPLE_RATE,AD7988_GRAVITY,g_SystemParam_Config.integ_freq_min,g_SystemParam_Config.integ_freq_max); // integral conf
	
	BSP_FFT_Calc(ad7988_float_accbuf, &fft_instance);
	trans485data.base_frequency = fft_instance.base_freq;
	trans485data.acc_peak = fft_instance.tim_domain_peak ;//* 0.0763f - 2500.0f;
	trans485data.acc_min =  fft_instance.tim_domain_min;
	trans485data.acc_mean = fft_instance.tim_domain_mean;
	trans485data.harmonic_peak_0_5 = BSP_GetHarmonicPeak(trans485data.base_frequency, 0.5,fft_instance.mag_pbuf) ; 
	trans485data.harmonic_peak_1 = BSP_GetHarmonicPeak(trans485data.base_frequency, 1,fft_instance.mag_pbuf) ;
	trans485data.harmonic_peak_1_5 = BSP_GetHarmonicPeak(trans485data.base_frequency, 1.5,fft_instance.mag_pbuf) ;
	trans485data.harmonic_peak_2 = BSP_GetHarmonicPeak(trans485data.base_frequency, 2,fft_instance.mag_pbuf) ;
	trans485data.harmonic_peak_2_5 = BSP_GetHarmonicPeak(trans485data.base_frequency, 2.5,fft_instance.mag_pbuf) ;
	trans485data.harmonic_peak_3 = BSP_GetHarmonicPeak(trans485data.base_frequency, 3,fft_instance.mag_pbuf) ;
	trans485data.harmonic_peak_4 = BSP_GetHarmonicPeak(trans485data.base_frequency, 4,fft_instance.mag_pbuf) ;
	trans485data.harmonic_peak_5 = BSP_GetHarmonicPeak(trans485data.base_frequency, 5,fft_instance.mag_pbuf) ;
	trans485data.harmonic_peak_6 = BSP_GetHarmonicPeak(trans485data.base_frequency, 6,fft_instance.mag_pbuf) ;
	
	
	// -----------Get RMS speed -----------------------
	//BSP_FrqDomain_Integral(1,fft_instance.fft_pbuf, ad7988_intrgralValue_space);
	trans485data.speed_peak = 0;
	
	uint32_t temp_1 = 0;
	//arm_max_f32( ad7988_intrgralValue_space + 1024,2048,&trans485data.speed_peak,&temp_1);
	trans485data.speed_peak = BSP_GetSpeedRMS(fft_instance.mag_pbuf,4096);
	trans485data.speed_peak = trans485data.speed_peak * 9.8f * 1000; // * 1000 -> m/s to mm/s;
	
	// --------------------------------------------
	// ----------Get max displacement------------------
	
	BSP_FrqDomain_Integral( 2 , fft_instance.fft_pbuf, ad7988_float_accbuf);
	float displace_temp_min = 0;
	float displace_temp_max = 0;
	for(uint16_t i  = 1024; i < 3072 ; i ++)
	{
		if(displace_temp_min > ad7988_float_accbuf[i])
		{
			displace_temp_min = ad7988_float_accbuf[i];
		}
		if(displace_temp_max < ad7988_float_accbuf[i])
		{
			displace_temp_max = ad7988_float_accbuf[i];
		}
	}
	
	//trans485data.displace_peak = 0;
	//arm_max_f32( ad7988_float_accbuf + 1024,2048,&trans485data.displace_peak,&temp_1);
	trans485data.displace_peak = displace_temp_max * 9.8f;
	//arm_max_f32( ad7988_float_accbuf + 1024,2048,&trans485data.displace_min,&temp_1);
	trans485data.displace_min = displace_temp_min * 9.8f;
	// ---------------------------------------------
	
	// -------------- check trans485data.base_frequency----------

	if( (trans485data.acc_peak * g_SystemParam_Config.AD7988_VolACC_p)  <= 4.0f)
	{
		trans485data.acc_peak = 0;
		trans485data.base_frequency = 1;
		trans485data.harmonic_peak_0_5 = 0;
		trans485data.harmonic_peak_1 = 0;
		trans485data.harmonic_peak_1_5 = 0;
		trans485data.harmonic_peak_2 = 0;
		trans485data.harmonic_peak_2_5 = 0;
		trans485data.harmonic_peak_3 = 0;
		trans485data.harmonic_peak_4 = 0;
		trans485data.harmonic_peak_5 = 0;
		trans485data.harmonic_peak_6 = 0;
		trans485data.displace_peak = 0;
		trans485data.speed_peak = 0;
		
		trans485data.acc_mean = 0;
		trans485data.acc_min = 0;
		trans485data.displace_min = 0;
	}

	// ----------------------------------------------------------
	
	
	// ---------------- Transfor ----------
	
	trans485data.power = 11;
	trans485data.temperature = T_4_20mv_GetTemperatureValue(); 
	
	
	      // ----DEBUG----
	char rtt_buf[40] = { 0 };
	snprintf(rtt_buf,40,"%f",trans485data.acc_peak);
	DEBUG("acc_peak:%s\r\n",rtt_buf);
	snprintf(rtt_buf,40,"%f",trans485data.base_frequency);
	DEBUG("base_frequency:%s\r\n",rtt_buf);	
	snprintf(rtt_buf,40,"%f",trans485data.displace_peak);
	DEBUG("offset_peak:%s\r\n",rtt_buf);	
	snprintf(rtt_buf,40,"%f",trans485data.speed_peak);
	DEBUG("speed_peak:%s\r\n",rtt_buf);	
	snprintf(rtt_buf,40,"%f",trans485data.temperature);
	DEBUG("temperature:%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_0_5);
	DEBUG("harmonic_peak_0_5 :%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_1);
	DEBUG("harmonic_peak_1   :%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_1_5);
	DEBUG("harmonic_peak_1.5 :%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_2);
	DEBUG("harmonic_peak_2   :%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_2_5);
	DEBUG("harmonic_peak_2_5 :%s\r\n",rtt_buf);	
	
	
		 // --------------	
	if(g_SystemParam_Config.Auto_Report_Flag == 1)
	{
		buf_ptr = Transfor_MakePackage(&len,&trans485data);
		Transfor_Send(buf_ptr,len);			
	}
	else
	{
		
	}

	// -----------------Start Sample--------- 
	//BSP_Tim_Start(BSP_TIM1);
	// --------------------------------------
}

Trans485_datavalue_t * AD7988_GetValue(void)
{
	return &trans485data;
	//return 0;
}

void AD7988_Status_CheckProcess(void)
{
	uint16_t status_advalue = 0;
	status_advalue = BSP_ADC_GetVolmvValue(BSP_ADC2);
	
	//DEBUG("AD7988 status_advalue:%d\r\n",status_advalue);
	
	if(status_advalue >=  3000)
	{
		ad7988_status = AD7988_Err1;
		BSP_LED_Close(BSP_LED_VIBRATOR);
	}
	else if(status_advalue <= 10)
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


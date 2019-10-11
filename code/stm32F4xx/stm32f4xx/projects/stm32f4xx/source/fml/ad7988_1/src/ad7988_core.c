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
static float ad7988_intrgralValue_space[AD7988_SAMPLE_LEN] = { 0 };
static uint16_t ad7988_fftbuf[2][AD7988_FFT_LENGTH] = { 0 };

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
	
	ad7988_fftbuf[current_read_buffer][ad_position ++] = *buf;
	if(ad_position >= AD7988_SAMPLE_RATE)
	{
		ad_position = 0;
		ad7988_valueIndex = current_read_buffer;
		current_read_buffer = !current_read_buffer;
		VibrateTask_Send_Event(VIBRATE_TASK_CALC_EVENT);// this place to triger some event
	}
}


void AD7988_Calc_Process(void)  
{
	uint16_t i = 0;
	uint8_t * buf_ptr = 0;
	uint16_t len = 0;
	Trans485_datavalue_t trans485data = { 0 };
	fft_instance_t fft_instance = { 0 };
	uint16_t *pread;
	pread = ad7988_fftbuf[ad7988_valueIndex];
	// ----------Stop Sample----------------------
	BSP_Tim_Stop(BSP_TIM1);
	// -------------------------------------------
	// ---------- fft calc------------------------

	BSP_FFT_IntegInit(AD7988_SAMPLE_LEN,AD7988_SAMPLE_RATE,AD7988_GRAVITY,AD7988_FRQ_MIN,AD7988_FRQ_MAX); // integral conf	

	BSP_FFT_Calc(pread, &fft_instance);
	
	trans485data.base_frequency = fft_instance.base_freq;
	trans485data.acc_peak = fft_instance.tim_domain_peak;
	
	trans485data.harmonic_peak_0_5 = BSP_GetHarmonicPeak(trans485data.base_frequency, 0.5,fft_instance.fft_pbuf);
	trans485data.harmonic_peak_1 = BSP_GetHarmonicPeak(trans485data.base_frequency, 1,fft_instance.fft_pbuf);
	trans485data.harmonic_peak_1_5 = BSP_GetHarmonicPeak(trans485data.base_frequency, 1.5,fft_instance.fft_pbuf);
	trans485data.harmonic_peak_2 = BSP_GetHarmonicPeak(trans485data.base_frequency, 2,fft_instance.fft_pbuf);
	trans485data.harmonic_peak_2_5 = BSP_GetHarmonicPeak(trans485data.base_frequency, 2.5,fft_instance.fft_pbuf);
	trans485data.harmonic_peak_3 = BSP_GetHarmonicPeak(trans485data.base_frequency, 3,fft_instance.fft_pbuf);
	trans485data.harmonic_peak_4 = BSP_GetHarmonicPeak(trans485data.base_frequency, 4,fft_instance.fft_pbuf);
	trans485data.harmonic_peak_5 = BSP_GetHarmonicPeak(trans485data.base_frequency, 5,fft_instance.fft_pbuf);
	trans485data.harmonic_peak_6 = BSP_GetHarmonicPeak(trans485data.base_frequency, 6,fft_instance.fft_pbuf);
	
	
	// -----------Get max speed -----------------------
	BSP_FrqDomain_Integral(1,fft_instance.fft_pbuf, ad7988_intrgralValue_space);
	trans485data.speed_peak = 0;
	for(i = 0;i < AD7988_SAMPLE_LEN; i ++)
	{
		if(trans485data.speed_peak < ad7988_intrgralValue_space[i])
		{
			trans485data.speed_peak = ad7988_intrgralValue_space[i];
		}
	}
	// --------------------------------------------
	// ----------Get max displacement------------------
	BSP_FrqDomain_Integral(2,fft_instance.fft_pbuf, ad7988_intrgralValue_space);
	trans485data.offset_peak = 0;
	for(i = 0;i < AD7988_SAMPLE_LEN; i ++)
	{
		if(trans485data.offset_peak < ad7988_intrgralValue_space[i])
		{
			trans485data.offset_peak = ad7988_intrgralValue_space[i];
		}
	}
	// ---------------------------------------------
	
	// ---------------- Transfor ----------
	
	trans485data.power = 11;
	trans485data.temperature = T_4_20mv_GetTemperatureValue(); 
	      // ----DEBUG----
	char rtt_buf[40] = { 0 };
	snprintf(rtt_buf,40,"%f",trans485data.acc_peak);
	DEBUG("acc_peak:%s\r\n",rtt_buf);
	snprintf(rtt_buf,40,"%f",trans485data.base_frequency);
	DEBUG("base_frequency:%s\r\n",rtt_buf);	
	snprintf(rtt_buf,40,"%f",trans485data.offset_peak);
	DEBUG("offset_peak:%s\r\n",rtt_buf);	
	snprintf(rtt_buf,40,"%f",trans485data.speed_peak);
	DEBUG("speed_peak:%s\r\n",rtt_buf);	
	snprintf(rtt_buf,40,"%f",trans485data.temperature);
	DEBUG("temperature:%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_0_5);
	DEBUG("harmonic_peak_0_5:%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_1);
	DEBUG("harmonic_peak_1:%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_1_5);
	DEBUG("harmonic_peak_1.5:%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_2);
	DEBUG("harmonic_peak_2:%s\r\n",rtt_buf);	
		snprintf(rtt_buf,40,"%f",trans485data.harmonic_peak_2_5);
	DEBUG("harmonic_peak_2_5:%s\r\n",rtt_buf);	
		 // --------------	
	buf_ptr = Transfor_MakePackage(&len,&trans485data);
	Transfor_Send(buf_ptr,len);	
	// -----------------Start Sample--------- 
	BSP_Tim_Start(BSP_TIM1);
	// --------------------------------------
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


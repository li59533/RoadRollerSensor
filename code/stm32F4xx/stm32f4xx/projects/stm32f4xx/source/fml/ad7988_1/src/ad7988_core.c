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
#include "transfor_makepackage.h"
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
//static float * ad7988_fft_inputbuf;
//static uint16_t * ad7988_fftbuf[2];
static uint8_t ad7988_status = 0;
static uint8_t ad7988_valueIndex = 0;

arm_cfft_radix2_instance_f32 ad7988_scfft;

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
static float ad7988_fft_inputbuf[AD7988_FFT_LENGTH * 2] = { 0 };
static uint16_t ad7988_fftbuf[2][AD7988_FFT_LENGTH] = { 0 };

void AD7988_ParamInit(void)
{
	//ad7988_fft_inputbuf = (float *)mymalloc(SRAMCCM,AD7988_FFT_LENGTH*sizeof(float) );
	//ad7988_fftbuf[0]= (uint16_t *)mymalloc(SRAMCCM,AD7988_FFT_LENGTH*sizeof(uint16_t) );
	//ad7988_fftbuf[1]= (uint16_t *)mymalloc(SRAMCCM,AD7988_FFT_LENGTH*sizeof(uint16_t) );
	
	arm_cfft_radix2_init_f32(&ad7988_scfft,AD7988_FFT_LENGTH,0,1);//初始化scfft结构体，设定FFT相关参数
	
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


//	传感器灵敏度参数
float Sense;
#define GAIN 10.0f
#define AD_REF_VOLT 5000.0f //mv
#define OUTPUT_MAG_RATIO 0x10000
#define OUTPUT_ANGLE_RATIO 0x10000
/* 变量 */
uint32_t fftSize = 1024;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;

static float32_t testOutput[AD7988_SAMPLE_RATE/2];

void AD7988_Calc_Process(void)  
{
	uint16_t i = 0;
	uint8_t * buf_ptr = 0;
	uint16_t len = 0;
	Trans485_datavalue_t teans485data = { 0 };
	BSP_Tim_Stop(BSP_TIM1);
	for(i = 0;i < AD7988_SAMPLE_RATE;i ++)
	{
		//DEBUG("%d  %d\r\n",i,ad7988_fftbuf[ad7988_valueIndex][i]);
	//	BSP_Systick_Delayms(3);
	}
	
	float ratio;
	unsigned short sTempreture;
	float fTempreture;
	float temp;
	uint32_t t;
	int16_t *ps;
	uint16_t *pread;
	uint16_t  min_acc;
	uint16_t  max_acc;	
	
	ratio = 1.0f/(Sense * GAIN);
	ratio *=(float)OUTPUT_MAG_RATIO;//0X10000表示1g

	pread = ad7988_fftbuf[ad7988_valueIndex];
	
	min_acc = 65535;
	max_acc = 0;

	for( i = 0;i < 1024;i ++)
	{
		 if(pread[i]>max_acc){
		 	max_acc = pread[i];
		 }
		 if(pread[i]<min_acc){
		 	min_acc = pread[i];
		 }			 
		 ad7988_fft_inputbuf[2*i]= ( (float)pread[i] ) *(AD_REF_VOLT/65535.0f)/((float)AD7988_FFT_LENGTH/2.0f) - AD_REF_VOLT/2.0f/((float)AD7988_FFT_LENGTH/2.0f);//毫伏值除以块长，预先除。
		 //ad7988_fft_inputbuf[2*i] = arm_sin_f32(2*3.1415926f*50*i/1000);
		 ad7988_fft_inputbuf[2*i+1] = 0;//虚部全部为0
	}	
	
	
	 /* CFFT变换 */
     arm_cfft_f32(&arm_cfft_sR_f32_len1024, ad7988_fft_inputbuf, ifftFlag, doBitReverse);

     /* 求解模值  */
     arm_cmplx_mag_f32(ad7988_fft_inputbuf, testOutput, fftSize);
	
	
	//arm_cfft_radix2_f32(&ad7988_scfft,ad7988_fft_inputbuf);	//FFT计算（基4）2ms
	//arm_cmplx_mag_f32(ad7988_fft_inputbuf,fft_outputbufMag,CANOPEN_LENGTH);	//把运算结果复数求模得幅值 
	
	/*
	for(i=0; i<1024; i++)
	{
		char test_buf[30] = { 0 };
		snprintf(test_buf,30,"%f",testOutput[i]);
		DEBUG("%s\r\n", test_buf);
	}
	 */
	teans485data.acc_pfak = 1;
	teans485data.base_frequency = 2;
	teans485data.harmonic_peak_0_5 = 3;
	teans485data.harmonic_peak_1 = 4;
	teans485data.harmonic_peak_1_5 = 5;
	teans485data.harmonic_peak_2 = 6;
	teans485data.harmonic_peak_3 = 7;
	teans485data.harmonic_peak_4 = 8;
	teans485data.harmonic_peak_5 = 9;
	teans485data.harmonic_peak_6 = 0;
	teans485data.offset_peak = 10;
	teans485data.power = 11;
	teans485data.speed_peak = 12;
	teans485data.temperature = 13;
	 
	buf_ptr = Transfor_MakePackage(&len,&teans485data);
	
	Transfor_Send(buf_ptr,len);
	 
	
	 
	BSP_Tim_Start(BSP_TIM1);
}

void AD7988_Status_CheckProcess(void)
{
	uint16_t status_advalue = 0;
	status_advalue = BSP_ADC_GetVolmvValue(BSP_ADC2);
	
	DEBUG("AD7988 status_advalue:%d\r\n",status_advalue);
	
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


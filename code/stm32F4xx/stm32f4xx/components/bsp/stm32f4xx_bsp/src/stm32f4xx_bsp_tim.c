/**
 **************************************************************************************************
 * @file        stm32f4xx_bsp_tim.c
 * @author
 * @version   v0.1.0
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */
#include "stm32f4xx_bsp_conf.h"

/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    stm32f4xx_bsp_tim_Modules 
 * @{  
 */

/**
 * @defgroup      stm32f4xx_bsp_tim_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_tim_Macros_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_tim_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_tim_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_tim_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */
 
/**
 * @defgroup      stm32f4xx_bsp_tim_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_tim_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      stm32f4xx_bsp_tim_Functions 
 * @brief         
 * @{  
 */
void BSP_Tim_Open(uint8_t BSP_TIMx,uint8_t *userparam)
{
	switch(BSP_TIMx)
	{
		case BSP_TIM1:
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure;

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTFʱ��	

			GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); //GPIOb5����Ϊ��ʱ��3

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           //GPIOb5
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
			GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��GPIOb5

			TIM_TimeBaseStructure.TIM_Prescaler=83;  //��ʱ����Ƶ  1000000/SAMPLE_RATE,84
			TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
			TIM_TimeBaseStructure.TIM_Period=487;   //�Զ���װ��ֵ
			TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
			TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��3

			//��ʼ��TIM3 Channel2 PWMģʽ	 
			TIM_OCInitStructure.TIM_Pulse = 244;	
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
			TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

			TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
			TIM_CtrlPWMOutputs(TIM3,  ENABLE);
			TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
		}
		break;
		default:break;
	}
}

void BSP_Tim_Stop(uint8_t BSP_TIMx)
{
	switch(BSP_TIMx)
	{
		case BSP_TIM1:
		{
			TIM_Cmd(TIM3, DISABLE);  //ʹ��TIM3
		}
		break;
		default:break;
	}
}
void BSP_Tim_Start(uint8_t BSP_TIMx)
{
	switch(BSP_TIMx)
	{
		case BSP_TIM1:
		{
			TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
		}
		break;
		default:break;
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


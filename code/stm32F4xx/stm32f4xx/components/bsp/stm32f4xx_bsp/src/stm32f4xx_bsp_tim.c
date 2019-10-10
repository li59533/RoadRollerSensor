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

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTF时钟	

			GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); //GPIOb5复用为定时器3

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           //GPIOb5
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
			GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化GPIOb5

			TIM_TimeBaseStructure.TIM_Prescaler=83;  //定时器分频  1000000/SAMPLE_RATE,84
			TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
			TIM_TimeBaseStructure.TIM_Period=487;   //自动重装载值
			TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
			TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器3

			//初始化TIM3 Channel2 PWM模式	 
			TIM_OCInitStructure.TIM_Pulse = 244;	
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
			TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

			TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
			TIM_CtrlPWMOutputs(TIM3,  ENABLE);
			TIM_Cmd(TIM3, ENABLE);  //使能TIM3
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
			TIM_Cmd(TIM3, DISABLE);  //使能TIM3
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
			TIM_Cmd(TIM3, ENABLE);  //使能TIM3
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


#include "bsp.h"
extern uint8_t IsEnterIRQ ;//收到数据标志位从spi接口接收到数据的标志位
/*********************************************************************************************************
*   函 数 名: GPIO_Configuration
*   功能说明: 初始化sx1278的中断引脚的GPIO功能，注意使能时钟
*********************************************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 						 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*********************************************************************************************************
*   函 数 名: EXTI_Configuration
*   功能说明: 中断控制初始化
*********************************************************************************************************/
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
  EXTI_ClearITPendingBit(EXTI_Line2);
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}
/*********************************************************************************************************
*   函 数 名: NVIC_Configuration
*   功能说明: 中断控制初始化
*********************************************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;  													
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	      
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}
/*********************************************************************************************************
*   函 数 名: EXTI2_IRQHandler
*   功能说明: 中断服务函数
*********************************************************************************************************/
void EXTI2_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line2) != RESET ) {
		EXTI_ClearITPendingBit(EXTI_Line2);
	}	
}


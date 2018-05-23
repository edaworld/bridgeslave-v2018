#include "bsp.h"
extern uint8_t IsEnterIRQ ;//�յ����ݱ�־λ��spi�ӿڽ��յ����ݵı�־λ
/*********************************************************************************************************
*   �� �� ��: GPIO_Configuration
*   ����˵��: ��ʼ��sx1278���ж����ŵ�GPIO���ܣ�ע��ʹ��ʱ��
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
*   �� �� ��: EXTI_Configuration
*   ����˵��: �жϿ��Ƴ�ʼ��
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
*   �� �� ��: NVIC_Configuration
*   ����˵��: �жϿ��Ƴ�ʼ��
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
*   �� �� ��: EXTI2_IRQHandler
*   ����˵��: �жϷ�����
*********************************************************************************************************/
void EXTI2_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line2) != RESET ) {
		EXTI_ClearITPendingBit(EXTI_Line2);
	}	
}


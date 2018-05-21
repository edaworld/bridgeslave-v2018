/********************************************************************************************************

    ģ������ : LEDģ��(For STM32F1XX)
    �ļ����� : bsp_led.c
    ˵    �� : ��ʼ��LED������Ҫ��GPIO�˿�
							 ����LED�ڵ�һЩ�궨����ͷ�ļ���
*********************************************************************************************************/
#include "bsp.h"
/*********************************************************************************************************
    �� �� ��: LED_Init
    ����˵��: ��ʼ��LED����Ӧ�����ţ��ֱ�ΪPA11��PA12��PB0��PB1
*********************************************************************************************************/
void LED_Init ( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd ( LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE );
    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( LED1_GPIO_PORT, &GPIO_InitStructure );
	
    GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
    GPIO_Init ( LED2_GPIO_PORT, &GPIO_InitStructure );
	
    GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
    GPIO_Init ( LED3_GPIO_PORT, &GPIO_InitStructure );
	
    GPIO_SetBits ( LED1_GPIO_PORT, LED1_GPIO_PIN );//�����øߣ�Ϩ��LED
    GPIO_SetBits ( LED2_GPIO_PORT, LED2_GPIO_PIN );//�����øߣ�Ϩ��LED
    GPIO_SetBits ( LED3_GPIO_PORT, LED3_GPIO_PIN );//�����øߣ�Ϩ��LED
}

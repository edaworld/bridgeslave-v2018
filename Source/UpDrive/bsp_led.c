/********************************************************************************************************

    模块名称 : LED模块(For STM32F1XX)
    文件名称 : bsp_led.c
    说    明 : 初始化LED灯所需要的GPIO端口
							 操作LED口的一些宏定义在头文件中
*********************************************************************************************************/
#include "bsp.h"
/*********************************************************************************************************
    函 数 名: LED_Init
    功能说明: 初始化LED所对应的引脚，分别为PA11、PA12和PB0、PB1
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
	
    GPIO_SetBits ( LED1_GPIO_PORT, LED1_GPIO_PIN );//引脚置高，熄灭LED
    GPIO_SetBits ( LED2_GPIO_PORT, LED2_GPIO_PIN );//引脚置高，熄灭LED
    GPIO_SetBits ( LED3_GPIO_PORT, LED3_GPIO_PIN );//引脚置高，熄灭LED
}

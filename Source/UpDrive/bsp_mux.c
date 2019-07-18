/*
	TS3A4751PWR�����߼�
	IN ======= NO to COM/COM to NO
	L          OFF
	H          ON  
*/
#include "bsp.h"
/*********************************************************************************************************
    �� �� ��: bsp_pztMux_Init
    ����˵��: ��ʼ��PZ��ʹ�õ�IO����
		��Ӧ��ϵ��Z1 ==== PB0
		          Z2 ==== PB1
		          Z3 ==== PB3
		          Z4 ==== PB4
		          Z5 ==== PB8
		          Z6 ==== PB9
		          Z7 ==== PB10
		          Z8 ==== PB11							
*********************************************************************************************************/
void bsp_pztMux_Init ( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    RCC_APB2PeriphClockCmd ( PZTMUX_GPIO_CLK , ENABLE );/*������ص�GPIO����ʱ��*/
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //	������PB3��PB4��PA15����ͨIO��PA13&14����SWD����
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;/*������������Ϊ50MHz */

    GPIO_InitStructure.GPIO_Pin = PZTMUX_GPIO_Z1 | PZTMUX_GPIO_Z2 | PZTMUX_GPIO_Z3 | PZTMUX_GPIO_Z4 | \
																	PZTMUX_GPIO_Z5 | PZTMUX_GPIO_Z6 | PZTMUX_GPIO_Z7 | PZTMUX_GPIO_Z8;/*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_Init ( PZTMUX_GPIO_PORT, &GPIO_InitStructure );/*���ÿ⺯������ʼ��GPIO*/
		
    GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
		GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
		GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
		GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
		GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
		GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
		GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
		GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );		
}

void pztMuxSwitch(uint8_t portnum)
{
    switch(portnum)
    {
    case PZT1:
        GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );				
        break;
    case PZT2:
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
        GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );
        break;
    case PZT3:
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
        GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );
        break;
    case PZT4:
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
				GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );
        break;
    case PZT5:
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
				GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );
        break;
    case PZT6:
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
				GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );
        break;
    case PZT7:
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
				GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );
        break;
    case PZT8:
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z1 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z2 );
        GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z3 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z4 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z5 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z6 );
				GPIO_ResetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z7 );
				GPIO_SetBits ( PZTMUX_GPIO_PORT, PZTMUX_GPIO_Z8 );
        break;
    }
}


/*********************************************************************************************************
    �� �� ��: bsp_rfbMux_Init
    ����˵��: ��ʼ��Rfb��ʹ�õ�IO����
		��Ӧ��ϵ��R1 ==== PA3
		          R2 ==== PA4
		          R3 ==== PA5
		          R4 ==== PA6
		          R5 ==== PA7
		          R6 ==== PA11
		          R7 ==== PA12
		          R8 ==== PA15							
*********************************************************************************************************/
void bsp_rfbMux_Init ( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    RCC_APB2PeriphClockCmd ( RFB_GPIO_CLK , ENABLE );/*������ص�GPIO����ʱ��*/
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;/*������������Ϊ50MHz */

    GPIO_InitStructure.GPIO_Pin = RFB_GPIO_R1 | RFB_GPIO_R2 | RFB_GPIO_R3 | RFB_GPIO_R4 |\
																	RFB_GPIO_R5 | RFB_GPIO_R6 | RFB_GPIO_R7 | RFB_GPIO_R8;/*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_Init ( RFB_GPIO_PORT, &GPIO_InitStructure );/*���ÿ⺯������ʼ��GPIO*/
		
    GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
		GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
		GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
		GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
    GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
		GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
		GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
		GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );				
}


void    rfbMuxSwitch(uint8_t portnum)
{
    switch(portnum)
    {
    case RFB_100_1K:
				GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );	
        break;
    case RFB_1K_10K:
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
				GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );
        break;
    case RFB_10K_50K:
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
				GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );
        break;
    case RFB_50K_100K:
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
				GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );
        break;
    case RFB_100K_500K:
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
				GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );
        break;
    case RFB_500K_1M:
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
				GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );
        break;
    case RFB_1M_10M:
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
				GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );
        break;
    case RFB_10M_50M:
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R1 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R2 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R3 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R4 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R5 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R6 );
				GPIO_ResetBits ( RFB_GPIO_PORT, RFB_GPIO_R7 );
				GPIO_SetBits ( RFB_GPIO_PORT, RFB_GPIO_R8 );
        break;
    }
}

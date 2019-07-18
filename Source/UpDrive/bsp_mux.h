#ifndef  __BSP_MUX_H
#define  __BSP_MUX_H
#include "stm32f10x.h" // Device header

#define PZTMUX_GPIO_PORT        GPIOB
#define PZTMUX_GPIO_CLK         RCC_APB2Periph_GPIOB
#define PZTMUX_GPIO_Z1         	GPIO_Pin_0
#define PZTMUX_GPIO_Z2         	GPIO_Pin_1
#define PZTMUX_GPIO_Z3         	GPIO_Pin_3
#define PZTMUX_GPIO_Z4         	GPIO_Pin_4
//�˴��Ķ��壬ԭ��ͼ��P10��ӦZ6��IO����PB9
#define PZTMUX_GPIO_Z5         	GPIO_Pin_9
//�˴��Ķ��壬ԭ��ͼ��P11��ӦZ7��IO����PB10
#define PZTMUX_GPIO_Z6         	GPIO_Pin_10
//�˴��Ķ��壬ԭ��ͼ��P12��ӦZ8��IO����PB11
#define PZTMUX_GPIO_Z7         	GPIO_Pin_11
//�˴��Ķ��壬ԭ��ͼ��P13��ӦZ5��IO����PB8
#define PZTMUX_GPIO_Z8         	GPIO_Pin_8

#define RFB_GPIO_PORT           GPIOA
#define RFB_GPIO_CLK            RCC_APB2Periph_GPIOA
#define RFB_GPIO_R1            	GPIO_Pin_3
#define RFB_GPIO_R2            	GPIO_Pin_4
#define RFB_GPIO_R3            	GPIO_Pin_5
#define RFB_GPIO_R4            	GPIO_Pin_6
#define RFB_GPIO_R5            	GPIO_Pin_7
#define RFB_GPIO_R6            	GPIO_Pin_11
#define RFB_GPIO_R7            	GPIO_Pin_12
#define RFB_GPIO_R8            	GPIO_Pin_15


//���¶��巴�������Ӧ��ţ���ԭ��ͼ����ֵ������1���ȼ����ֱ��10����1M
#define RFB_100_1K          0
#define RFB_1K_10K          1
#define RFB_10K_50K         2
#define RFB_50K_100K        3
#define RFB_100K_500K       4
#define RFB_500K_1M         5
#define RFB_1M_10M          6
#define RFB_10M_50M         7
//���¶������˿ں�
#define PZT1        0
#define PZT2        1
#define PZT3        2
#define PZT4        3
#define PZT5        4
#define PZT6        5
#define PZT7        6
#define PZT8        7

void    bsp_pztMux_Init(void); //����Ӧ���ⲿ����ڳ�ʼ������
void    bsp_rfbMux_Init(void); //����Ӧ�ķ����������ڳ�ʼ������
void    pztMuxSwitch(uint8_t portnum);
void    rfbMuxSwitch(uint8_t portnum);

#endif


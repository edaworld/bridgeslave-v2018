#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H
#include "bsp.h"

void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void *_pCallBack); //ʹ��TIM2-5�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�����������ͬʱ����4����ʱ�����������š�

void bsp_HardTimerInit(void);//����TIMx������us����Ӳ����ʱ��TIMx���������У�����ֹͣ.

#endif

/********************************************************************************************************
*
*   ģ������ : Ӳ����ʱ��
*   �ļ����� : bsp_timer.c
*   ��    �� : V1.0
*   ˵    �� :
*   �޸ļ�¼ :

*********************************************************************************************************/

#include "bsp.h"

/*
  ��������Ӳ����ʱ����TIM�� ����ʹ TIM2 - TIM5
    TIM3 ��TIM4 ��16λ
    TIM2 ��TIM5 ��16λ (103��16λ, 407��32λ)
*/
#define USE_TIM2
//#define USE_TIM3
//#define USE_TIM4
//#define USE_TIM5

#ifdef USE_TIM2
#define TIM_HARD        TIM2
#define TIM_HARD_IRQn   TIM2_IRQn
#define TIM_HARD_RCC    RCC_APB1Periph_TIM2
#endif

#ifdef USE_TIM3
#define TIM_HARD        TIM3
#define TIM_HARD_IRQn   TIM3_IRQn
#define TIM_HARD_RCC    RCC_APB1Periph_TIM3
#endif

#ifdef USE_TIM4
#define TIM_HARD        TIM4
#define TIM_HARD_IRQn   TIM4_IRQn
#define TIM_HARD_RCC    RCC_APB1Periph_TIM4
#endif

#ifdef USE_TIM5
#define TIM_HARD        TIM5
#define TIM_HARD_IRQn   TIM5_IRQn
#define TIM_HARD_RCC    RCC_APB1Periph_TIM5
#endif


/* ���� TIM��ʱ�жϵ���ִ�еĻص�����ָ�� */
static void (*s_TIM_CallBack1)(void);
static void (*s_TIM_CallBack2)(void);
static void (*s_TIM_CallBack3)(void);
static void (*s_TIM_CallBack4)(void);

/*
*********************************************************************************************************
*   �� �� ��: bsp_HardTimerInit
*   ����˵��: ���� TIMx������us����Ӳ����ʱ��TIMx���������У�����ֹͣ.
*           TIMx������TIM2 - TIM5 ֮���TIM, ��ЩTIM��4��ͨ��, ���� APB1 �ϣ�����ʱ��=SystemCoreClock / 2
*   ��    ��: ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
#if defined (USE_TIM2) || defined (USE_TIM3)  || defined (USE_TIM4) || defined (USE_TIM5)
void bsp_HardTimerInit(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    uint32_t usPeriod;
    uint16_t usPrescaler;
    uint32_t uiTIMxCLK;

    /* ʹ��TIMʱ�� */
    RCC_APB1PeriphClockCmd(TIM_HARD_RCC, ENABLE);

    /*-----------------------------------------------------------------------
        system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

        HCLK = SYSCLK / 1     (AHB1Periph)
        PCLK2 = HCLK / 2      (APB2Periph)
        PCLK1 = HCLK / 4      (APB1Periph)

        ��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
        ��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

        APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
        APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

    ----------------------------------------------------------------------- */
    uiTIMxCLK = SystemCoreClock / 2;

    usPrescaler = uiTIMxCLK / 1000000 ; /* ��Ƶ������ 1us */

#if defined (USE_TIM2) || defined (USE_TIM5)
    //usPeriod = 0xFFFFFFFF;    /* 407֧��32λ��ʱ�� */
    usPeriod = 0xFFFF;  /* 103֧��16λ */
#else
    usPeriod = 0xFFFF;
#endif
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = usPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM_HARD, &TIM_TimeBaseStructure);

    //TIM_ARRPreloadConfig(TIMx, ENABLE);

    /* TIMx enable counter */
    TIM_Cmd(TIM_HARD, ENABLE);

    /* ����TIM��ʱ�ж� (Update) */
    {
        NVIC_InitTypeDef NVIC_InitStructure;    /* �жϽṹ���� misc.h �ж��� */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
        NVIC_InitStructure.NVIC_IRQChannel = TIM_HARD_IRQn;

        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   /* �ȴ������ȼ��� */
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}

/*
*********************************************************************************************************
*   �� �� ��: bsp_StartHardTimer
*   ����˵��: ʹ��TIM2-5�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�����������ͬʱ����4����ʱ�����������š�
*           ��ʱ��������10us ����Ҫ�ķ��ڵ��ñ�������ִ��ʱ�䣬�����ڲ������˲�����С��
*           TIM2��TIM5 ��16λ��ʱ����
*           TIM3��TIM4 ��16λ��ʱ����
*   ��    ��: _CC : ����ͨ������1��2��3, 4
*           _uiTimeOut : ��ʱʱ��, ��λ 1us.  ����16λ��ʱ������� 65.5ms; ����32λ��ʱ������� 4294��
*           _pCallBack : ��ʱʱ�䵽�󣬱�ִ�еĺ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void *_pCallBack)
{
    uint32_t cnt_now;
    uint32_t cnt_tar;

    /*
        ִ�����������䣬ʱ�� = 18us (ͨ���߼������ǲ���IO��ת)
        bsp_StartTimer2(3, 500, (void *)test1);
    */
    if (_uiTimeOut < 5)
    {
        ;
    }
    else
    {
        _uiTimeOut -= 5;
    }

    cnt_now = TIM_GetCounter(TIM_HARD);     /* ��ȡ��ǰ�ļ�����ֵ */
    cnt_tar = cnt_now + _uiTimeOut;         /* ���㲶��ļ�����ֵ */
    if (_CC == 1)
    {
        s_TIM_CallBack1 = (void (*)(void))_pCallBack;

        TIM_SetCompare1(TIM_HARD, cnt_tar);       /* ���ò���Ƚϼ�����CC1 */
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC1);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC1, ENABLE);   /* ʹ��CC1�ж� */

    }
    else if (_CC == 2)
    {
        s_TIM_CallBack2 = (void (*)(void))_pCallBack;

        TIM_SetCompare2(TIM_HARD, cnt_tar);       /* ���ò���Ƚϼ�����CC2 */
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC2);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC2, ENABLE);   /* ʹ��CC2�ж� */
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;

        TIM_SetCompare3(TIM_HARD, cnt_tar);       /* ���ò���Ƚϼ�����CC3 */
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC3);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC3, ENABLE);   /* ʹ��CC3�ж� */
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack4 = (void (*)(void))_pCallBack;

        TIM_SetCompare4(TIM_HARD, cnt_tar);       /* ���ò���Ƚϼ�����CC4 */
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC4);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC4, ENABLE);   /* ʹ��CC4�ж� */
    }
    else
    {
        return;
    }
}

#endif

/*
*********************************************************************************************************
*   �� �� ��: TIMx_IRQHandler
*   ����˵��: TIM �жϷ������
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/

#ifdef USE_TIM2
void TIM2_IRQHandler(void)
#endif

#ifdef USE_TIM3
void TIM3_IRQHandler(void)
#endif

#ifdef USE_TIM4
void TIM4_IRQHandler(void)
#endif

#ifdef USE_TIM5
void TIM5_IRQHandler(void)
#endif
{
    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC1);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC1, DISABLE);    /* ����CC1�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC2);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC2, DISABLE);    /* ����CC2�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC3);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC3, DISABLE);    /* ����CC3�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC4);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC4, DISABLE);    /* ����CC4�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack4();
    }
}

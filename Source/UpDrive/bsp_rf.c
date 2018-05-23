/*********************************************************************************************************
    ģ������ : RFģ��(For STM32F1XX)
    �ļ����� : bsp_rf.c
    ˵    �� :
*********************************************************************************************************/
#include "bsp.h"
const char *rfName = "SX1278";
//u16 iSend, iRev;    //���߷��ͺͽ��ռ���
u8  sendBuf[64];    //���ͻ�����
u8  revBuf[128];    //���ջ�����
/*********************************************************************************************************
    �� �� ��: RFGPIOInit
    ����˵��: ��ʼ��sx1278�������������ţ��ֱ���IRQ��NREST��Csn���ţ�ע����ʹ��ʱ��
*********************************************************************************************************/
void RFGPIOInit ( void )
{
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );//ע��ʹ��GPIO��ʱ��
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );//ע��ʹ��GPIO��ʱ��    
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure PA8 NREST as Output push-pull ��λ���ţ�Ϊ�������*/
    //��������ԭ��ͼ����NREST����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );
    /* Configure PB12 RF_Csn as Output push-pull Ƭѡ���ţ�Ϊ�������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init ( GPIOB, &GPIO_InitStructure );
    /* Configure PA2 RF_IRQ as input �ж����ţ�Ϊ��������*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );

}

/*********************************************************************************************************
    �� �� ��: RFInit
    ����˵��: ��ʼ��sx1278��SPI����
*********************************************************************************************************/
void RFInit ( void )
{
    SPI2_Init();
    RFM96_LoRaEntryRx(); //�������ģʽ
}
/*********************************************************************************************************
    �� �� ��: RFRxMode
    ����˵��: ��Ƶģ��������ģʽ
*********************************************************************************************************/
void RFRxMode ( void )
{
    RFM96_LoRaEntryRx(); //�������ģʽ
}
/*********************************************************************************************************
    �� �� ��: RFRevData
    ����˵��: 
*********************************************************************************************************/
u8 RFRevData()// �˴���������
{
    u8 length = 0;
    if (GPIO_ReadInputDataBit(GPIOA,RF_IRQ_PIN)) //�յ����ݸߵ�ƽ�ж�
    {
        length = RFM96_LoRaRxPacket(revBuf);
        RFRxMode();
    }
//    if (length > 0)
//    {
//        iRev++;
//    } //�������ݸ���
    return (length);
}
/*********************************************************************************************************
    �� �� ��: RFSendData
    ����˵��: sx1278�������ݺ���
*********************************************************************************************************/
u8 RFSendData ( u8 *buf, u8 size )
{
    int ret = 0;
    ret = RFM96_LoRaEntryTx ( size ); //���ط����ֽ���
    ret = RFM96_LoRaTxPacket ( buf, size ); //���ط����ֽ���
    bsp_DelayMS ( 5 );
    RFRxMode(); //�������ģʽ
//    if ( ret > 0 )
//    {
//        iSend++;//�������ݸ���
//    }    
    return ( ret ); //�ɹ������0��ֵ
}



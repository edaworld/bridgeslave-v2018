/*********************************************************************************************************
    模块名称 : RF模块(For STM32F1XX)
    文件名称 : bsp_rf.c
    说    明 :
*********************************************************************************************************/
#include "bsp.h"
const char *rfName = "SX1278";
//u16 iSend, iRev;    //无线发送和接收计数
u8  sendBuf[64];    //发送缓冲区
u8  revBuf[128];    //接收缓冲区
/*********************************************************************************************************
    函 数 名: RFGPIOInit
    功能说明: 初始化sx1278的三个控制引脚，分别是IRQ，NREST和Csn引脚，注意先使能时钟
*********************************************************************************************************/
void RFGPIOInit ( void )
{
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );//注意使能GPIO口时钟
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );//注意使能GPIO口时钟    
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure PA8 NREST as Output push-pull 复位引脚，为输出引脚*/
    //此引脚在原理图中是NREST引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );
    /* Configure PB12 RF_Csn as Output push-pull 片选引脚，为输出引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init ( GPIOB, &GPIO_InitStructure );
    /* Configure PA2 RF_IRQ as input 中断引脚，为输入引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );

}

/*********************************************************************************************************
    函 数 名: RFInit
    功能说明: 初始化sx1278的SPI引脚
*********************************************************************************************************/
void RFInit ( void )
{
    SPI2_Init();
    RFM96_LoRaEntryRx(); //进入接收模式
}
/*********************************************************************************************************
    函 数 名: RFRxMode
    功能说明: 射频模块进入接收模式
*********************************************************************************************************/
void RFRxMode ( void )
{
    RFM96_LoRaEntryRx(); //进入接收模式
}
/*********************************************************************************************************
    函 数 名: RFRevData
    功能说明: 
*********************************************************************************************************/
u8 RFRevData()// 此处参数无用
{
    u8 length = 0;
    if (GPIO_ReadInputDataBit(GPIOA,RF_IRQ_PIN)) //收到数据高电平中断
    {
        length = RFM96_LoRaRxPacket(revBuf);
        RFRxMode();
    }
//    if (length > 0)
//    {
//        iRev++;
//    } //接收数据个数
    return (length);
}
/*********************************************************************************************************
    函 数 名: RFSendData
    功能说明: sx1278发送数据函数
*********************************************************************************************************/
u8 RFSendData ( u8 *buf, u8 size )
{
    int ret = 0;
    ret = RFM96_LoRaEntryTx ( size ); //返回发送字节数
    ret = RFM96_LoRaTxPacket ( buf, size ); //返回发送字节数
    bsp_DelayMS ( 5 );
    RFRxMode(); //进入接收模式
//    if ( ret > 0 )
//    {
//        iSend++;//发送数据个数
//    }    
    return ( ret ); //成功后大于0的值
}



/******************** (C) COPYRIGHT tongxinmao.com ***************************
* �ļ���        : SPI2.C
* ��������      : 2016.3.8
* ����          : SPI2�����ļ�
********************************************************************************/
#include "bsp.h"
#define RF_SX1278
/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
*******************************************************************************/
void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_I2S_DeInit(SPI2);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
    //SPI2��ʹ�õ���GPIOB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    /* Configure SPI2 pins: SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //PB13ΪSPI2_CLK��PB14ΪSPI2_MISO��PB15ΪSPI2_MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* SPI1 configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;        // 
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;        // 
#ifdef RF_SI4463
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;// 
#endif
#if (defined RF_SI4432) || (defined RF_SX1278)
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;  // ����ģʽ�� SCK Ϊ��
#endif
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//SPI_CPHA_2Edge; //
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //
    SPI_InitStructure.SPI_CRCPolynomial = 7; //
    SPI_Init(SPI2, &SPI_InitStructure); //
    /* Enable SPI2  */
    SPI_Cmd(SPI2, ENABLE); //
}
u8 SPI_ExchangeByte( u8 input )
{
    /* Loop while DR register in not emplty */
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI2, input);
    /* Wait to receive a byte */
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);
}


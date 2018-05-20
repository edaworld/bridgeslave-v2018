/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   i2c EEPROM(AT24C02)Ӧ�ú���bsp
  */

#include "bsp.h"

/* STM32 I2C ����ģʽ */
#define I2C_Speed              400000  //*
 
/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
#define I2Cx_OWN_ADDRESS7      0X7C   //
//#define I2Cx_OWN_ADDRESS7      0X0A   //

/* AT24C01/02ÿҳ��8���ֽ� */
#define I2C_PageSize           8

/* AT24C04/08A/16Aÿҳ��16���ֽ� */
//#define I2C_PageSize           16

uint16_t EEPROM_ADDRESS;


/**
  * @brief  I2C1 I/O����
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* ʹ���� I2C1 �йص�ʱ�� */
    macI2C_APBxClock_FUN ( macI2C_CLK, ENABLE );
    macI2C_GPIO_APBxClock_FUN ( macI2C_GPIO_CLK, ENABLE );


    /* PB6-I2C1_SCL��PB7-I2C1_SDA*/
    GPIO_InitStructure.GPIO_Pin = macI2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;        // ��©���
    GPIO_Init(macI2C_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = macI2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;        // ��©���
    GPIO_Init(macI2C_SDA_PORT, &GPIO_InitStructure);
}


/**
  * @brief  I2C ����ģʽ����
  * @param  ��
  * @retval ��
  */
static void I2C_Mode_Configu(void)
{
    I2C_InitTypeDef  I2C_InitStructure;

    /* I2C ���� */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;

    /* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;

    I2C_InitStructure.I2C_OwnAddress1 = I2Cx_OWN_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;

    /* I2C��Ѱַģʽ */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    /* ͨ������ */
    I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

    /* I2C1 ��ʼ�� */
    I2C_Init(macI2Cx, &I2C_InitStructure);

    /* ʹ�� I2C1 */
    I2C_Cmd(macI2Cx, ENABLE);
}


/**
  * @brief  I2C ����(EEPROM)��ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_EE_Init(void)
{

    I2C_GPIO_Config();

    I2C_Mode_Configu();

    /* ����ͷ�ļ�i2c_ee.h�еĶ�����ѡ��EEPROMҪд��ĵ�ַ */
#ifdef macEEPROM_Block0_ADDRESS
    /* ѡ�� EEPROM Block0 ��д�� */
    EEPROM_ADDRESS = macEEPROM_Block0_ADDRESS;
#endif

#ifdef macEEPROM_Block1_ADDRESS
    /* ѡ�� EEPROM Block1 ��д�� */
    EEPROM_ADDRESS = macEEPROM_Block1_ADDRESS;
#endif

#ifdef macEEPROM_Block2_ADDRESS
    /* ѡ�� EEPROM Block2 ��д�� */
    EEPROM_ADDRESS = macEEPROM_Block2_ADDRESS;
#endif

#ifdef macEEPROM_Block3_ADDRESS
    /* ѡ�� EEPROM Block3 ��д�� */
    EEPROM_ADDRESS = macEEPROM_Block3_ADDRESS;
#endif
}



/**
  * @brief   дһ���ֽڵ�I2C EEPROM��
  * @param
  *     @arg pBuffer:������ָ��
  *     @arg WriteAddr:д��ַ
  * @retval  ��
  */
void I2C_EE_ByteWrite( u8 WriteAddr, u8 dat )
{
    /* Send STRAT condition */
    I2C_GenerateSTART(macI2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(macI2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Send the EEPROM's internal address to write to */
    I2C_SendData(macI2Cx, WriteAddr);

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send the byte to be written */
    I2C_SendData(macI2Cx, dat);

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STOP condition */
    I2C_GenerateSTOP(macI2Cx, ENABLE);
}

/**
  * @brief   ��EEPROM�����ȡһ������
  * @param
  *     @arg pBuffer:��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ��
  *     @arg WriteAddr:�������ݵ�EEPROM�ĵ�ַ
  *     @arg NumByteToWrite:Ҫ��EEPROM��ȡ���ֽ���
  * @retval  ��
  */
unsigned char  I2C_EE_ByteRead(u8 ReadAddr )
{
    unsigned char tempx = 0;

    //*((u8 *)0x4001080c) |=0x80;
    while(I2C_GetFlagStatus(macI2Cx, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008

    /* Send START condition */
    I2C_GenerateSTART(macI2Cx, ENABLE);
    //*((u8 *)0x4001080c) &=~0x80;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(macI2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(macI2Cx, ENABLE);

    /* Send the EEPROM's internal address to write to */
    I2C_SendData(macI2Cx, 0xB0);  //----AD5933��ָ�����2017-4-11
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send the EEPROM's internal address to write to */
    I2C_SendData(macI2Cx, ReadAddr);
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    //--bug 2017-4-12 for generate stop!!!!!!!!
    /* Send STOP condition */
    I2C_GenerateSTOP(macI2Cx, ENABLE);

    //---------------------------------������ȡ����---------------------------------//
    /* Send STRAT condition a second time */
    I2C_GenerateSTART(macI2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(macI2Cx, EEPROM_ADDRESS, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    I2C_AcknowledgeConfig(macI2Cx, ENABLE);//add by xiaojun 2017-4-13

    while(1)
    {
        /* Disable Acknowledgement */
        //I2C_AcknowledgeConfig(macI2Cx, DISABLE);
        /* Send STOP Condition */
        //I2C_GenerateSTOP(macI2Cx, ENABLE);

        if(I2C_CheckEvent(macI2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            tempx = I2C_ReceiveData(macI2Cx);
            I2C_AcknowledgeConfig(macI2Cx, DISABLE);   //Bug 2017-4-12 add this for end
            I2C_GenerateSTOP(macI2Cx, ENABLE);          //Bug 2017-4-12 add this for end
            break;
        }

    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(macI2Cx, ENABLE);

    return tempx;
}


/*********************************************END OF FILE**********************/


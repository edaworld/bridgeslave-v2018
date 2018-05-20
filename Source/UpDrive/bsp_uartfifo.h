/********************************************************************************************************
*
*   ģ������ : �����ж�+FIFO����ģ��
*   �ļ����� : Uart_fifo.h
*   ��    �� : V1.0
*   ˵    �� : ͷ�ļ�
*
*********************************************************************************************************/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_
#include "bsp.h"
/*
    �����Ҫ���Ĵ��ڶ�Ӧ�Ĺܽţ��������޸� Uart_fifo.c�ļ��е� static void UART_InitHardPara(void)����
*/

/* ����ʹ�ܵĴ���, 0 ��ʾ��ʹ�ܣ������Ӵ����С���� 1��ʾʹ�� */
/*
    USBPC��·�崮�ڷ��䣺
    ������1�� CH340TоƬ��1·�����Խ׶���Ϊ��Ϣ�����
        PA9/USART1_TX
        PA10/USART1_RX

    ������2��SX1278 ģ��
        PA2/USART2_TX
        PA3/USART2_RX

    ������3�� GPS ͨ�ţ���USBPC���޷�ʹ�ã�
        PB10/USART3_TX
        PB11/USART3_RX
        PB2/BOOT1/RS485_TX_EN

    ������4�� --- ���������á�
    ������5�� --- ���������á�
*/
#define UART1_FIFO_EN   1
#define UART2_FIFO_EN   0
#define UART3_FIFO_EN   0
#define UART4_FIFO_EN   0
#define UART5_FIFO_EN   0

/* RS485оƬ����ʹ��GPIO, PB2 */
#define RCC_RS485_TXEN   RCC_APB2Periph_GPIOB
#define PORT_RS485_TXEN  GPIOB
#define PIN_RS485_TXEN   GPIO_Pin_2

#define RS485_RX_EN()   PORT_RS485_TXEN->BRR = PIN_RS485_TXEN
#define RS485_TX_EN()   PORT_RS485_TXEN->BSRR = PIN_RS485_TXEN


/* ����˿ں� */
typedef enum
{
    COM1 = 0,   /* USART1  PA9, PA10 */
    COM2 = 1,   /* USART2, PA2, PA3 */
    COM3 = 2,   /* USART3, PB10, PB11 */
    COM4 = 3,   /* UART4, PC10, PC11 */
    COM5 = 4,   /* UART5, PC12, PD2 */
} COM_PORT_E;

/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART1_FIFO_EN == 1
#define UART1_BAUD          115200
#define UART1_TX_BUF_SIZE   1*1024
#define UART1_RX_BUF_SIZE   1*1024
#endif

#if UART2_FIFO_EN == 1
#define UART2_BAUD          115200
#define UART2_TX_BUF_SIZE   1*1024
#define UART2_RX_BUF_SIZE   1*1024
#endif

#if UART3_FIFO_EN == 1
#define UART3_BAUD          115200
#define UART3_TX_BUF_SIZE   1*1024
#define UART3_RX_BUF_SIZE   1*1024
#endif

#if UART4_FIFO_EN == 1
#define UART4_BAUD          115200
#define UART4_TX_BUF_SIZE   1*1024
#define UART4_RX_BUF_SIZE   1*1024
#endif

#if UART5_FIFO_EN == 1
#define UART5_BAUD          115200
#define UART5_TX_BUF_SIZE   1*1024
#define UART5_RX_BUF_SIZE   1*1024
#endif

/* �����豸�ṹ�� */
typedef struct
{
    USART_TypeDef *uart;        /* STM32�ڲ������豸ָ�� */
    uint8_t *pTxBuf;            /* ���ͻ����� */
    uint8_t *pRxBuf;            /* ���ջ����� */
    uint16_t usTxBufSize;       /* ���ͻ�������С */
    uint16_t usRxBufSize;       /* ���ջ�������С */
    __IO uint16_t usTxWrite;            /* ���ͻ�����дָ�� */
    __IO uint16_t usTxRead;         /* ���ͻ�������ָ�� */
    __IO uint16_t usTxCount;            /* �ȴ����͵����ݸ��� */

    __IO uint16_t usRxWrite;            /* ���ջ�����дָ�� */
    __IO uint16_t usRxRead;         /* ���ջ�������ָ�� */
    __IO uint16_t usRxCount;            /* ��δ��ȡ�������ݸ��� */

    void (*SendBefore)(void);   /* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
    void (*SendOver)(void);     /* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
    void (*ReciveNew)(uint8_t _byte);   /* �����յ����ݵĻص�����ָ�� */
} UART_T;

void UART_InitALL(void);
void COMx_SendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);   //_ucPort���ںţ�_ucaBuf���ڷ��ͻ�������_usLen���ݳ���
void COMx_SendChar(COM_PORT_E _ucPort, uint8_t _ucByte);    //_ucPort���ںţ�_ucByte���ڷ����ֽ�����
uint8_t COMx_GetChar(COM_PORT_E _ucPort, uint8_t *_pByte);  //_ucPort���ںţ�_pByte���ڽ��ջ�����

void COMx_ClearTxFifo(COM_PORT_E _ucPort);
void COMx_ClearRxFifo(COM_PORT_E _ucPort);

void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);
void RS485_SendStr(char *_pBuf);

void UART3_SetBaud(uint32_t _baud);
void UART1_SetBaud(uint32_t _baud);
void UART2_SetBaud(uint32_t _baud);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

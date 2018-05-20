#include "bsp.h"

//extern uint8_t g_uart1_timeout; //��⴮��1�������ݳ�ʱ��ȫ�ֱ�������bsp_slavemsg.c�ļ�������
//extern uint8_t g_uart2_timeout; //��⴮��2�������ݳ�ʱ��ȫ�ֱ���
//extern RECVDATA_T g_tUart1; //��ʼ���Ӵ���1��BLE�������ݽṹ�壬��bsp_slavemsg.c�ļ�������
//extern RECVDATA_T g_tUart2; //��ʼ���Ӵ���2��Lora�������ݽṹ�壬��bsp_slavemsg.c�ļ�������

extern uint8_t TPCTaskNum; //������������bsp_task.c�б���ʼ����bsp_tpc.c��ʹ��
extern uint8_t IsEnterIRQ;  //���lora��IRQ���ţ���״̬�仯��Ϊ���յ�lora���ݣ���λ

uint8_t IsReceiveHostData = FALSE; //��ȷ�������������ݰ���־λ
SLAVEDATA s_SlaveData; //�ӻ����͵������Ľṹ��
READVALUE s_ReadData; //����ʵ�����鲿�Ľṹ��
static uint32_t measureNumbers;//��������
uint8_t isSingbleMeasureFlag = FALSE;

TPC_TASK TaskComps[3] =
{
	//���������ʱ����ע�ⵥ�������иı��������ԵĴ���
	{ 0, 0, 1, 5, Task_RecvfromHost }, // ��̬���񣬴����SPI�ӿڵ�SX127 8���յ���������ʱ��Ƭ���Ｔ��ִ��
	{ 1, 0, 1, 5, Task_SendToHost }, // ��̬�����յ��㲥�źţ����ʹӻ����ݵ�����
	{ 1, 0, 1, 100, Task_ReadAD5933 }, // ��ȡAD5933����    
};

/*********************************************************************************************************
    �� �� ��: Task_RecvfromLora
    ����˵��: �����SPI�ӿڵ�Lora���յ���������
*********************************************************************************************************/
static uint8_t recvdatbuffer[32];   //�������ݻ�����
void Task_RecvfromHost(void)
{
	static uint8_t ucrevcount;	
	uint8_t length;
	if(IsEnterIRQ == TRUE)    //�������м�⵽�ж�����Ϊ�ߣ���ʾ���յ����ݺ���λ�ñ�־λ
	{
		length = RFM96_LoRaRxPacket(recvdatbuffer);
		RFRxMode();
		if(length > 0)
		{
			ucrevcount++;
		}
		if((recvdatbuffer[0] == '%') && (recvdatbuffer[8] == '&'))
		{
			IsReceiveHostData = TRUE;   //���ý��յ�����������־λ
			TaskComps[1].attrb = 0; //���ڵ㷢����������Ϊ��̬����
		}
		IsEnterIRQ = FALSE;
	}
}

/*********************************************************************************************************
    �� �� ��: Task_SendToMaster
    ����˵��: �������ݰ�����������
*********************************************************************************************************/
void Task_SendToHost(void)
{
	if(IsReceiveHostData == TRUE)    //���յ��������͹������ź�
	{
		if(recvdatbuffer[1] == 0xEF)    //����ָ��
		{
			s_SlaveData.msg[1] = 0xEF;
			s_SlaveData.msg[2] = 0x01;
	}
		if(recvdatbuffer[1] == 0x7F)    //������ʼƵ�ʡ���ֹƵ�ʡ�����������PZT���迹����
		{
			memcpy(s_SlaveData.msg, recvdatbuffer, 12);
			AD5933_Set_Freq_Start(s_SlaveData.msg[2]);
			AD5933_Set_Freq_Num(s_SlaveData.data);
			measureNumbers = s_SlaveData.data;
			AD5933_Set_Freq_Add((s_SlaveData.msg[3] - s_SlaveData.msg[2]) * 1000 / s_SlaveData.data);
			pztMuxSwitch(s_SlaveData.msg[9]);
			rfbMuxSwitch(s_SlaveData.msg[10]);
			s_SlaveData.msg[1] = 0x7F;
			s_SlaveData.msg[2] = 0x01;
		}
		if(recvdatbuffer[1] == 0x3F)    //���õ�Ƶ����ָ��,Ƶ��Ϊ��ʼƵ��
		{
			isSingbleMeasureFlag = 1;
			AD5933_Set_Mode_Freq_Repeat();
			s_SlaveData.msg[1] = 0x3F;
			s_SlaveData.msg[2] = 0x01;
		}
		if(recvdatbuffer[1] == 0x1F)    //����ɨƵ����ָ��
		{
			isSingbleMeasureFlag = 0;
			AD5933_Set_Mode_Freq_UP();
			s_SlaveData.msg[1] = 0x1F;
			s_SlaveData.msg[2] = 0x01;
		}
		if(recvdatbuffer[1] == 0x0F)    //�������ָ��
		{
			s_SlaveData.msg[1] = 0x0F;
			s_SlaveData.msg[2] = 0x01;
			Init_AD5933();
			AD5933_Set_Mode_Freq_Start();
			TaskComps[2].attrb = 0;
		}
		if(recvdatbuffer[1] == 0x03)   //ֹͣ���ָ��
		{
			s_SlaveData.msg[1] = 0x03;
			s_SlaveData.msg[2] = 0x01;
//			AD5933_Set_Mode_Rst();//��λ���������û��ж�ɨ��
			AD5933_Set_Mode_Standby();
		}
		s_SlaveData.head = '&';
		s_SlaveData.tail = '%';
		RFSendData(s_SlaveData.msg, 12);   
		mem_set(s_SlaveData.msg, 0, 12);   
		TaskComps[1].attrb = 1; 
		IsReceiveHostData = FALSE;
	}
	LED1_TOGGLE(); //��ɫD3
}
/*********************************************************************************************************
    �� �� ��: Task_ReadAD5933
    ����˵��: ��ȡAD5933���迹��������
*********************************************************************************************************/
void Task_ReadAD5933(void)
{
	static unsigned char temp;
	static unsigned int real, img;
	static uint32_t readAD5933count;	
	DISABLE_INT();
	temp = AD5933_Get_DFT_ST();//��ȡ״̬�Ĵ�����0x04��ɨ����ɣ�0x02��ʵ�����鲿��Ч
	ENABLE_INT();
	if(isSingbleMeasureFlag == 0)
	{
		if((temp & 0x04) != 0x04)//ɨ��δ���ʱ
		{
			if(temp & 0x02)    //ʵ�����鲿��Ч
			{
				s_ReadData.head = '&';
				s_ReadData.tail = '%';
				s_ReadData.msg[1] = 0x07;
				s_ReadData.msg[2] = 0x01;
				real = AD5933_Get_Real();
				img  = AD5933_Get_Img();
				s_ReadData.real = real;
				s_ReadData.img = img;
				readAD5933count ++;							
				s_ReadData.counts = readAD5933count;//����ȡ������������λ������ͳ��
				s_ReadData.bitofover = 0x55;
				if(readAD5933count == measureNumbers)
					s_ReadData.bitofover = 0xAA;
				RFSendData(s_ReadData.msg, 12);
				AD5933_Set_Mode_Freq_UP();
			}
		}
		else if((temp & 0x04) == 0x04)
		{
			TaskComps[2].attrb = 1;
			readAD5933count = 0;
		}
	}
	if(isSingbleMeasureFlag == 1)
	{
		if(temp & 0x02)    //ʵ�����鲿��Ч
		{
			s_ReadData.head = '&';
			s_ReadData.tail = '%';
			s_ReadData.msg[1] = 0x07;
			s_ReadData.msg[2] = 0x01;
			real = AD5933_Get_Real();
			img  = AD5933_Get_Img();
			s_ReadData.real = real;
			s_ReadData.img = img;
			readAD5933count ++;
			s_ReadData.counts = readAD5933count;//����ȡ������������λ������ͳ��
			s_ReadData.bitofover = 0x55;//���ȱ�־λ����ʾ���ڲ���������
			if(readAD5933count == measureNumbers)
				s_ReadData.bitofover = 0xAA;//��ʾ��������	
			RFSendData(s_ReadData.msg, 12);
			AD5933_Set_Mode_Freq_Repeat();            
		}
		if(readAD5933count == measureNumbers)
		{
			AD5933_Set_Mode_Freq_UP();
			TaskComps[2].attrb = 1;
			readAD5933count = 0;
		}
	}
}
/*********************************************************************************************************
    �� �� ��: TaskInit
    ����˵��: �����ʼ��
*********************************************************************************************************/
void TaskInit(void)
{
    TPCTaskNum = (sizeof(TaskComps) / sizeof(TaskComps[0]));       // ��ȡ������
}

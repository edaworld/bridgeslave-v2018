#include "bsp.h"  // Device header

static uint8_t recvlength;
extern u8  revBuf[128];    //���ջ�����
SLAVEDATA s_SlaveData; //�ӻ����͵������Ľṹ��
READVALUE s_ReadData; //����ʵ�����鲿�Ľṹ��
static uint32_t measureNumbers;//��������

static uint8_t isSingbleMeasureFlag = 0;//0����ʾɨƵ������1����ʾ��Ƶ����
static uint8_t isReadingAD5933 = 0;//0����ʾ������1����ʾ�����ȡ

static unsigned char temp;//��ʱ�洢��ȡAD5933������
static unsigned int real, img;//���ʵ�����鲿����
static uint32_t readAD5933count;//��Ŷ�ȡ����

int main(void)
{
	bsp_Init();  //��ʼ��Ӳ���豸
	bsp_DelayMS(10);		
	while(1)
	{ 						
		recvlength = RFRevData();
		if(recvlength > 0)
		{
			if((revBuf[0] == '%') && (revBuf[8] == '&'))//Host----->Slave�İ�ͷ%�Ͱ�β&��ȷ 
			{
				//����ָ��
				if(revBuf[1] == 0xEF)    
				{
					s_SlaveData.msg[1] = 0xEF;
					s_SlaveData.msg[2] = 0x01;
				}
				//������ʼƵ�ʡ���ֹƵ�ʡ�����������PZT���迹����
				if(revBuf[1] == 0x7F)    
				{
					memcpy(s_SlaveData.msg, revBuf, 12);
					AD5933_Set_Freq_Start(s_SlaveData.msg[2]);
					AD5933_Set_Freq_Num(s_SlaveData.data);
					measureNumbers = s_SlaveData.data;//�����˲����Ĵ���
					AD5933_Set_Freq_Add((s_SlaveData.msg[3] - s_SlaveData.msg[2]) * 1000 / s_SlaveData.data);
					pztMuxSwitch(s_SlaveData.msg[9]);
					rfbMuxSwitch(s_SlaveData.msg[10]);
					s_SlaveData.msg[1] = 0x7F;
					s_SlaveData.msg[2] = 0x01;
				}
				//���õ�Ƶ����ָ��,Ƶ��Ϊ��ʼƵ��
				if(revBuf[1] == 0x3F)    
				{
					isSingbleMeasureFlag = 1;
					AD5933_Set_Mode_Freq_Repeat();
					s_SlaveData.msg[1] = 0x3F;
					s_SlaveData.msg[2] = 0x01;
				}
				//����ɨƵ����ָ��
				if(revBuf[1] == 0x1F)    
				{
					isSingbleMeasureFlag = 0;
					AD5933_Set_Mode_Freq_UP();
					s_SlaveData.msg[1] = 0x1F;
					s_SlaveData.msg[2] = 0x01;
				}
				//�������ָ��
				if(revBuf[1] == 0x0F)    
				{
					s_SlaveData.msg[1] = 0x0F;
					s_SlaveData.msg[2] = 0x01;
					Init_AD5933();
					AD5933_Set_Mode_Freq_Start();
					isReadingAD5933 = 1;
				}
				if(revBuf[1] == 0x03)   //ֹͣ���ָ��
				{
					s_SlaveData.msg[1] = 0x03;
					s_SlaveData.msg[2] = 0x01;
//					AD5933_Set_Mode_Rst();//��λ���������û��ж�ɨ��
					AD5933_Set_Mode_Standby();
				}

				s_SlaveData.head = '&';//Slave----->Host�İ�ͷ&�Ͱ�β��ֵ
				s_SlaveData.tail = '%';				
				RFSendData(s_SlaveData.msg,recvlength);
				mem_set(s_SlaveData.msg, 0, 12);
				mem_set(revBuf,0,128);
				recvlength = 0;				
			}
		}

		if(isReadingAD5933 == 1)
		{
	
			DISABLE_INT();
			temp = AD5933_Get_DFT_ST();//��ȡ״̬�Ĵ�����0x04��ɨ����ɣ�0x02��ʵ�����鲿��Ч
			ENABLE_INT();
			if(isSingbleMeasureFlag == 0)//ɨƵ����ģʽ
			{
				if((temp & 0x04) != 0x04)//ɨ��δ���ʱ
				{
					if(temp & 0x02)    //ʵ�����鲿��Ч
					{
						s_ReadData.head = '&';//Slave----->Host�İ�ͷ&�Ͱ�β��ֵ
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
						if(readAD5933count == measureNumbers)//��ȡ��ָ���������ݰ�
						{
							s_ReadData.bitofover = 0xAA;//���ݲɼ�������־
						}
						RFSendData(s_ReadData.msg, 12);
						AD5933_Set_Mode_Freq_UP();
					}
				}
				else if((temp & 0x04) == 0x04)//ɨƵ�������
				{				
					readAD5933count = 0;
					isReadingAD5933 = 0;					
				}
			}
			if(isSingbleMeasureFlag == 1)//��Ƶ����ģʽ
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
					isReadingAD5933 = 0;
					readAD5933count = 0;
				}
			}
		}		
	}
}

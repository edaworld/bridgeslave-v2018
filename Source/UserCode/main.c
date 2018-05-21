#include "bsp.h"  // Device header

uint8_t IsEnterIRQ = FALSE;//收到数据标志位，
static uint8_t recvlength;
extern u8  revBuf[128];    //接收缓冲区
extern SLAVEDATA s_SlaveData; //从机发送到主机的结构体
static READVALUE s_ReadData; //发送实部和虚部的结构体
static uint32_t measureNumbers;//测量点数
static uint8_t isSingbleMeasureFlag = 0;//0：表示扫频测量，1：表示单频测量


/*********************************************************************************************************
    函 数 名: ReadAD5933
    功能说明: 读取AD5933电阻抗数据函数
*********************************************************************************************************/
void ReadAD5933(void)
{
	static unsigned char temp;
	static unsigned int real, img;
	static uint32_t readAD5933count;	
	DISABLE_INT();
	temp = AD5933_Get_DFT_ST();//读取状态寄存器，0x04是扫描完成，0x02是实部和虚部有效
	ENABLE_INT();
	if(isSingbleMeasureFlag == 0)//扫频测量模式
	{
		if((temp & 0x04) != 0x04)//扫描未完成时
		{
			if(temp & 0x02)    //实部和虚部有效
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
				s_ReadData.counts = readAD5933count;//将读取次数发送至上位机进行统计
				s_ReadData.bitofover = 0x55;
				if(readAD5933count == measureNumbers)
					s_ReadData.bitofover = 0xAA;
				RFSendData(s_ReadData.msg, 12);
				AD5933_Set_Mode_Freq_UP();
			}
		}
		else if((temp & 0x04) == 0x04)
		{
			readAD5933count = 0;
		}
	}
	if(isSingbleMeasureFlag == 1)//单频测量模式
	{
		if(temp & 0x02)    //实部和虚部有效
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
			s_ReadData.counts = readAD5933count;//将读取次数发送至上位机进行统计
			s_ReadData.bitofover = 0x55;//进度标志位，表示正在测量过程中
			if(readAD5933count == measureNumbers)
				s_ReadData.bitofover = 0xAA;//表示测量结束	
			RFSendData(s_ReadData.msg, 12);
			AD5933_Set_Mode_Freq_Repeat();            
		}
		if(readAD5933count == measureNumbers)
		{
			AD5933_Set_Mode_Freq_UP();
			readAD5933count = 0;
		}
	}
}

int main(void)
{
	bsp_Init();  //初始化硬件设备
	bsp_DelayMS(10);
//	TaskInit(); //初始化任务,仅是获取任务数量，任务调度在bsp_idle中调用task_process实现
		
	while(1)
	{ 						
//		bsp_Idle();	
		
		recvlength = RFRevData();
		if(recvlength > 0)
		{
			if((revBuf[0] == '%') && (revBuf[8] == '&'))//Host----->Slave的包头%和包尾&正确 
			{
				//握手指令
				if(revBuf[1] == 0xEF)    
				{
					s_SlaveData.msg[1] = 0xEF;
					s_SlaveData.msg[2] = 0x01;
				}
				//设置起始频率、截止频率、测量点数、PZT和阻抗量程
				if(revBuf[1] == 0x7F)    
				{
					memcpy(s_SlaveData.msg, revBuf, 12);
					AD5933_Set_Freq_Start(s_SlaveData.msg[2]);
					AD5933_Set_Freq_Num(s_SlaveData.data);
					measureNumbers = s_SlaveData.data;//保存了测量的次数
					AD5933_Set_Freq_Add((s_SlaveData.msg[3] - s_SlaveData.msg[2]) * 1000 / s_SlaveData.data);
					pztMuxSwitch(s_SlaveData.msg[9]);
					rfbMuxSwitch(s_SlaveData.msg[10]);
					s_SlaveData.msg[1] = 0x7F;
					s_SlaveData.msg[2] = 0x01;
				}
				//设置单频测量指令,频率为起始频率
				if(revBuf[1] == 0x3F)    
				{
					isSingbleMeasureFlag = 1;
					AD5933_Set_Mode_Freq_Repeat();
					s_SlaveData.msg[1] = 0x3F;
					s_SlaveData.msg[2] = 0x01;
				}
				//设置扫频测量指令
				if(revBuf[1] == 0x1F)    
				{
					isSingbleMeasureFlag = 0;
					AD5933_Set_Mode_Freq_UP();
					s_SlaveData.msg[1] = 0x1F;
					s_SlaveData.msg[2] = 0x01;
				}
				//启动监测指令
				if(revBuf[1] == 0x0F)    
				{
					s_SlaveData.msg[1] = 0x0F;
					s_SlaveData.msg[2] = 0x01;
					Init_AD5933();
					AD5933_Set_Mode_Freq_Start();
				}
				if(revBuf[1] == 0x03)   //停止监测指令
				{
					s_SlaveData.msg[1] = 0x03;
					s_SlaveData.msg[2] = 0x01;
//					AD5933_Set_Mode_Rst();//复位命令允许用户中断扫描
					AD5933_Set_Mode_Standby();
				}
				
				
				s_SlaveData.head = '&';//Slave----->Host的包头&和包尾赋值
				s_SlaveData.tail = '%';				
				RFSendData(s_SlaveData.msg,recvlength);
				mem_set(s_SlaveData.msg, 0, 12);
				mem_set(revBuf,0,128);
				recvlength = 0;				
			}
		}		
	}
}

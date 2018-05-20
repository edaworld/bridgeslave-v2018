#include "bsp.h"

//extern uint8_t g_uart1_timeout; //检测串口1接收数据超时的全局变量，在bsp_slavemsg.c文件中声明
//extern uint8_t g_uart2_timeout; //检测串口2接收数据超时的全局变量
//extern RECVDATA_T g_tUart1; //初始化从串口1，BLE接收数据结构体，在bsp_slavemsg.c文件中声明
//extern RECVDATA_T g_tUart2; //初始化从串口2，Lora接收数据结构体，在bsp_slavemsg.c文件中声明

extern uint8_t TPCTaskNum; //任务数量，在bsp_task.c中被初始化，bsp_tpc.c中使用
extern uint8_t IsEnterIRQ;  //监测lora的IRQ引脚，有状态变化即为接收到lora数据，置位

uint8_t IsReceiveHostData = FALSE; //正确接收主机的数据包标志位
SLAVEDATA s_SlaveData; //从机发送到主机的结构体
READVALUE s_ReadData; //发送实部和虚部的结构体
static uint32_t measureNumbers;//测量点数
uint8_t isSingbleMeasureFlag = FALSE;

TPC_TASK TaskComps[3] =
{
	//添加新任务时，请注意单个任务中改变任务属性的代码
	{ 0, 0, 1, 5, Task_RecvfromHost }, // 静态任务，处理从SPI接口的SX127 8接收的数据任务，时间片到达即可执行
	{ 1, 0, 1, 5, Task_SendToHost }, // 动态任务，收到广播信号，发送从机数据到主机
	{ 1, 0, 1, 100, Task_ReadAD5933 }, // 读取AD5933任务    
};

/*********************************************************************************************************
    函 数 名: Task_RecvfromLora
    功能说明: 处理从SPI接口的Lora接收的数据任务
*********************************************************************************************************/
static uint8_t recvdatbuffer[32];   //接收数据缓冲区
void Task_RecvfromHost(void)
{
	static uint8_t ucrevcount;	
	uint8_t length;
	if(IsEnterIRQ == TRUE)    //主函数中检测到中断引脚为高，表示接收到数据后，置位该标志位
	{
		length = RFM96_LoRaRxPacket(recvdatbuffer);
		RFRxMode();
		if(length > 0)
		{
			ucrevcount++;
		}
		if((recvdatbuffer[0] == '%') && (recvdatbuffer[8] == '&'))
		{
			IsReceiveHostData = TRUE;   //设置接收到的主机包标志位
			TaskComps[1].attrb = 0; //将节点发送任务设置为静态任务
		}
		IsEnterIRQ = FALSE;
	}
}

/*********************************************************************************************************
    函 数 名: Task_SendToMaster
    功能说明: 发送数据包至主机任务
*********************************************************************************************************/
void Task_SendToHost(void)
{
	if(IsReceiveHostData == TRUE)    //接收到主机发送过来的信号
	{
		if(recvdatbuffer[1] == 0xEF)    //握手指令
		{
			s_SlaveData.msg[1] = 0xEF;
			s_SlaveData.msg[2] = 0x01;
	}
		if(recvdatbuffer[1] == 0x7F)    //设置起始频率、截止频率、测量点数、PZT和阻抗量程
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
		if(recvdatbuffer[1] == 0x3F)    //设置单频测量指令,频率为起始频率
		{
			isSingbleMeasureFlag = 1;
			AD5933_Set_Mode_Freq_Repeat();
			s_SlaveData.msg[1] = 0x3F;
			s_SlaveData.msg[2] = 0x01;
		}
		if(recvdatbuffer[1] == 0x1F)    //设置扫频测量指令
		{
			isSingbleMeasureFlag = 0;
			AD5933_Set_Mode_Freq_UP();
			s_SlaveData.msg[1] = 0x1F;
			s_SlaveData.msg[2] = 0x01;
		}
		if(recvdatbuffer[1] == 0x0F)    //启动监测指令
		{
			s_SlaveData.msg[1] = 0x0F;
			s_SlaveData.msg[2] = 0x01;
			Init_AD5933();
			AD5933_Set_Mode_Freq_Start();
			TaskComps[2].attrb = 0;
		}
		if(recvdatbuffer[1] == 0x03)   //停止监测指令
		{
			s_SlaveData.msg[1] = 0x03;
			s_SlaveData.msg[2] = 0x01;
//			AD5933_Set_Mode_Rst();//复位命令允许用户中断扫描
			AD5933_Set_Mode_Standby();
		}
		s_SlaveData.head = '&';
		s_SlaveData.tail = '%';
		RFSendData(s_SlaveData.msg, 12);   
		mem_set(s_SlaveData.msg, 0, 12);   
		TaskComps[1].attrb = 1; 
		IsReceiveHostData = FALSE;
	}
	LED1_TOGGLE(); //蓝色D3
}
/*********************************************************************************************************
    函 数 名: Task_ReadAD5933
    功能说明: 读取AD5933电阻抗数据任务
*********************************************************************************************************/
void Task_ReadAD5933(void)
{
	static unsigned char temp;
	static unsigned int real, img;
	static uint32_t readAD5933count;	
	DISABLE_INT();
	temp = AD5933_Get_DFT_ST();//读取状态寄存器，0x04是扫描完成，0x02是实部和虚部有效
	ENABLE_INT();
	if(isSingbleMeasureFlag == 0)
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
			TaskComps[2].attrb = 1;
			readAD5933count = 0;
		}
	}
	if(isSingbleMeasureFlag == 1)
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
			TaskComps[2].attrb = 1;
			readAD5933count = 0;
		}
	}
}
/*********************************************************************************************************
    函 数 名: TaskInit
    功能说明: 任务初始化
*********************************************************************************************************/
void TaskInit(void)
{
    TPCTaskNum = (sizeof(TaskComps) / sizeof(TaskComps[0]));       // 获取任务数
}

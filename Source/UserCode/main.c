#include "bsp.h"  // Device header

uint8_t IsEnterIRQ = FALSE;//收到数据标志位，
static uint8_t recvlength;
extern u8  revBuf[128];    //接收缓冲区


int main(void)
{
	bsp_Init();  //初始化硬件设备
	bsp_DelayMS(10);
	TaskInit(); //初始化任务,仅是获取任务数量，任务调度在bsp_idle中调用task_process实现
		
	while(1)
	{ 						
//		bsp_Idle();	
		
		recvlength = RFRevData();
		if(recvlength > 0)
		{		
			RFSendData(revBuf,recvlength);
			recvlength = 0;
		}				
	}
}

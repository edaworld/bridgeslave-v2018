#include "bsp.h"  // Device header

uint8_t IsEnterIRQ = FALSE;//�յ����ݱ�־λ��
static uint8_t recvlength;
extern u8  revBuf[128];    //���ջ�����


int main(void)
{
	bsp_Init();  //��ʼ��Ӳ���豸
	bsp_DelayMS(10);
	TaskInit(); //��ʼ������,���ǻ�ȡ�������������������bsp_idle�е���task_processʵ��
		
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

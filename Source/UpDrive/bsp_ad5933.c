#include "bsp.h"

#define AD5933_MCLK 16.776  //=536870912/MCLK; 16.776对应的是内部频率
unsigned int iMode;

void Init_AD5933 ( void )
{
    volatile int i, j, k;
//  AD5933_Set_Freq_Start ( 20000 ); //100Khz, 100K/1024 = 100Hz, data update rate is 100Hz!!!!!!
//  AD5933_Set_Freq_Add ( 100 ); //设置频率增量为1
//  AD5933_Set_Freq_Num ( 500 ); //设置增量数，也就是扫描中的频率点数
	
	//add by xiaojun for output spped 2018-4-29
	//for ADC sampling about 1ms, here set the wait time, All time = wait time + ADC sampling time
	//wait time = stimuls T (10us) * register
	I2C_EE_ByteWrite( 0x8A, 0x02);//建立时间周期数0x20: multiple by 2,0x06:mulitple by 4
	I2C_EE_ByteWrite( 0x8B, 200 );//建立时间周期数		
	
	AD5933_Set_Mode( AD5933_Standby, AD5933_OUTPUT_2V, AD5933_Gain_1, AD5933_IN_MCLK, AD5933_NReset );
	AD5933_Set_Mode_Rst();
	AD5933_Set_Mode_SysInit();
	//delay for enough time
	for ( i = 0; i < 1000; i++ )
		for ( j = 0; j < 1000; j++ )
		{
			k = 0;
		}
}

void Fre_To_Hex ( float fre, u8 *buf )
{
	u32 dat;
	dat = ( 536870912 / ( double ) ( AD5933_MCLK * 1000000 ) ) * fre;
	buf[0] = dat >> 16;
	buf[1] = dat >> 8;
	buf[2] = dat;
}
/*********************************************************************************************************
    函数名: AD5933_Set_Freq_Start
    功能说明:82、83、84设置起始频率
**********************************************************************************************************/
void AD5933_Set_Freq_Start ( unsigned int freq ) //此处已经在Fre_To_Hex中实现了频率值到写入值得转换
{
	unsigned char buf[3];
	freq = freq *1024;//由于上位机是以K为单位来传送的
	Fre_To_Hex ( freq, buf );
	I2C_EE_ByteWrite ( 0x82, buf[0] );
	I2C_EE_ByteWrite ( 0x83, buf[1] );
	I2C_EE_ByteWrite ( 0x84, buf[2] );
}
/*********************************************************************************************************
    函数名: AD5933_Set_Freq_Add
    功能说明:设置频率增量
**********************************************************************************************************/
void AD5933_Set_Freq_Add ( unsigned int afreq ) // 85,86,87设置频率增量
{
	unsigned char buf[3];
	Fre_To_Hex ( afreq, buf );
	I2C_EE_ByteWrite ( 0x85, buf[0] );
	I2C_EE_ByteWrite ( 0x86, buf[1] );
	I2C_EE_ByteWrite ( 0x87, buf[2] );
}
/*********************************************************************************************************
    函数名: AD5933_Set_Freq_Num
    功能说明:设置增量数
**********************************************************************************************************/
void AD5933_Set_Freq_Num ( unsigned int num ) //88,89设置增量数
{
	unsigned char buf[2];
	buf[0] = num >> 8 ;
	buf[1] = num;
	I2C_EE_ByteWrite ( 0x88, buf[0] );
	I2C_EE_ByteWrite ( 0x89, buf[1] );
}
/*********************************************************************************************************
    函数名: AD5933_Set_Mode
    功能说明:设置工作模式，输出激励电压，PGA增益，时钟选择及复位等
**********************************************************************************************************/
void AD5933_Set_Mode ( unsigned int ctrl, unsigned int out, unsigned int gain, unsigned int clk, unsigned int rst ) //80,81控制寄存器
{
	iMode = ctrl | out | gain | clk | rst;
	I2C_EE_ByteWrite ( 0x80, iMode >> 8 );
	I2C_EE_ByteWrite ( 0x81, iMode );
}

void AD5933_Set_Mode_Rst ( void ) //复位操作，D4位是复位控制位
{
	iMode  = ( iMode & 0xffef ) | AD5933_Reset;
	I2C_EE_ByteWrite ( 0x81, iMode );
}
/*********************************************************************************************************
    函数名: AD5933_Set_Mode_Standby
    功能说明:设置工作模式，初始化/启动/递增频率/重复频率/测温/省电/待机
**********************************************************************************************************/
void AD5933_Set_Mode_Standby ( void )
{
	iMode  = ( iMode & 0x0fff ) | AD5933_Standby;
	I2C_EE_ByteWrite ( 0x80, iMode >> 8 );
}

void AD5933_Set_Mode_SysInit ( void )
{
	iMode  = ( iMode & 0x0fff ) | AD5933_SYS_Init;
	I2C_EE_ByteWrite ( 0x80, iMode >> 8 );
}

void AD5933_Set_Mode_Freq_Start ( void )
{
	iMode  = ( iMode & 0x0fff ) | AD5933_Begin_Fre_Scan;
	I2C_EE_ByteWrite ( 0x80, iMode >> 8 );
}

void AD5933_Set_Mode_Freq_UP ( void )
{
	iMode  = ( iMode & 0x0fff ) | AD5933_Fre_UP;
	I2C_EE_ByteWrite ( 0x80, iMode >> 8 );
}

void AD5933_Set_Mode_Freq_Repeat ( void )
{
	iMode  = ( iMode & 0x0fff ) | AD5933_Fre_Rep;
	I2C_EE_ByteWrite ( 0x80, iMode >> 8 );
}

void AD5933_Set_Mode_Freq_Temp ( void )
{
	iMode  = ( iMode & 0x0fff ) | AD5933_Get_Temp;
	I2C_EE_ByteWrite ( 0x80, iMode >> 8 );
}
/*********************************************************************************************************
    函数名: AD5933_Get_DFT_ST
    功能说明:读取状态，温度测量有效/实值和虚值有效/频率扫描完成
    形参:
    返回值:
**********************************************************************************************************/
unsigned char  AD5933_Get_DFT_ST ( void )
{
	unsigned char temp;
	temp = I2C_EE_ByteRead ( 0x8F );
	return temp;
}
/*********************************************************************************************************
    函数名: AD5933_Get_Real
    功能说明:读取实部
    形参:
    返回值:
**********************************************************************************************************/
unsigned int AD5933_Get_Real ( void )
{
	unsigned char high, low;
	unsigned int real;
	high = I2C_EE_ByteRead ( 0x94 );
	low = I2C_EE_ByteRead ( 0x95 );
	real = ( high << 8 ) | low;
	return real;
}
/*********************************************************************************************************
    函数名: AD5933_Get_Img
    功能说明:读取虚部
    形参:
    返回值:
**********************************************************************************************************/
unsigned int AD5933_Get_Img ( void )
{
	unsigned char high, low;
	unsigned int img;
	high = I2C_EE_ByteRead ( 0x96 );
	low = I2C_EE_ByteRead ( 0x97 );
	img = ( high << 8 ) | low;
	return img;
}

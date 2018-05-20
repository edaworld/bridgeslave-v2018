#ifndef  __TPC__H__
#define  __TPC__H__
#include "stdint.h"
/********************************************************************************************************
* Variable definition
********************************************************************************************************/
typedef struct _TPC_TASK
{
    uint8_t   attrb;  //静态任务：0，动态任务：1
    uint8_t   Run;  // 程序运行标记，0：不运行，1：运行
    uint16_t  Timer;  // 计时器
    uint16_t  ItvTime;  // 任务运行间隔时间
    void      (*Task)(void); // 要运行的任务函数

} TPC_TASK; // 任务定义

//-------------------------------------------------------------------------------------------------------
typedef enum
{
    TPC_RUN_CLM = 0x00,                                             // 停止
    TPC_RUN_STM = 0x01,                                           // 运行

} TPC_RUN_MARK;

//-------------------------------------------------------------------------------------------------------
extern uint8_t TPCTaskNum;  // 任务数

/********************************************************************************************************
* Global function
********************************************************************************************************/
extern void TPCRemarks(TPC_TASK *pTask);  // 标志处理(节拍中断函数中调用)
extern void TPCProcess(TPC_TASK *pTask);  // 任务处理(主函数中调用)

/*******************************************************************************************************/
#endif

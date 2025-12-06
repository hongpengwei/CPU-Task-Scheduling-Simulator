// system.h
#ifndef SYSTEM_H
#define SYSTEM_H

#include "task.h"
#include "queue.h"

// 全域系統變數
extern int GLOBAL_TIME;
extern int TIME_QUANTUM;
extern Queue ReadyQueue;
extern Queue SleepList;

// 系統初始化
void SystemInit();

// 系統呼叫：睡眠
void SysCall_Sleep(Task* t, int duration);

// 硬體中斷模擬：檢查鬧鐘
void CheckAlarm();

#endif
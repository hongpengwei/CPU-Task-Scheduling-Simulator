// scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include "queue.h"

// 將任務放入 ReadyQueue (會根據演算法決定怎麼放)
void AddToReadyQueue(Task* t);

// 獲取下一個任務
Task* GetNextTask();

// 設定當前的排程演算法
void SetSchedulerType(SchedulerType type);
SchedulerType GetSchedulerType();

#endif
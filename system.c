// system.c
#include <stdio.h>
#include "system.h"
#include "scheduler.h" // 為了呼叫 AddToReadyQueue

// 定義全域變數
int GLOBAL_TIME = 0;
int TIME_QUANTUM = 2;
Queue ReadyQueue;
Queue SleepList;

void SystemInit() {
    GLOBAL_TIME = 0;
    initQueue(&ReadyQueue);
    initQueue(&SleepList);
}

void SysCall_Sleep(Task* t, int duration) {
    printf("    [SYSCALL] Task %s calls Sleep(%d). Wake up at tick %d.\n", 
           t->name, duration, GLOBAL_TIME + duration);
    
    t->state = SLEEPING;
    t->wake_up_time = GLOBAL_TIME + duration;
    
    // 依喚醒時間排序插入 SleepList
    insertSorted(&SleepList, t, 0);
}

void CheckAlarm() {
    // 檢查 SleepList 頭部任務是否時間到了
    while (!isEmpty(&SleepList) && SleepList.head->wake_up_time <= GLOBAL_TIME) {
        Task* woke_task = dequeue(&SleepList);
        printf("    [ALARM] Task %s woke up! Moving to ReadyQueue.\n", woke_task->name);
        
        // 喚醒後，根據當前排程規則放回 ReadyQueue
        AddToReadyQueue(woke_task);
    }
}
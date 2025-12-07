// scheduler.c
#include "scheduler.h"
#include "system.h" // 需要存取全域變數 TIME_QUANTUM
#include <stdlib.h> // <-- 新增這一行
static SchedulerType CURRENT_TYPE = SCHED_RR;

// 引用 system.c 中的 ReadyQueue
extern Queue ReadyQueue; 
extern int TIME_QUANTUM;

void SetSchedulerType(SchedulerType type) {
    CURRENT_TYPE = type;
}

SchedulerType GetSchedulerType() {
    return CURRENT_TYPE;
}

void AddToReadyQueue(Task* t) {
    t->state = READY;
    if (CURRENT_TYPE == SCHED_SJF) {
        insertSorted(&ReadyQueue, t, 1); // 按剩餘時間排序
    } else if (CURRENT_TYPE == SCHED_PRIORITY) {
        insertSorted(&ReadyQueue, t, 2); // 按優先權排序
    }else if (CURRENT_TYPE == SCHED_LIFO) {
        // LIFO: 將新任務放在隊列頭部
        t->next = ReadyQueue.head;
        ReadyQueue.head = t;
        if (ReadyQueue.tail == NULL) ReadyQueue.tail = t;
    } 
    else {
        enqueue(&ReadyQueue, t); // FCFS 和 RR 直接排隊尾
    }
}

Task* GetNextTask() {
    if (isEmpty(&ReadyQueue)) return NULL;

    Task* next = dequeue(&ReadyQueue);
    
    // 如果是 Round Robin，重置時間片
    if (CURRENT_TYPE == SCHED_RR) {
        next->rr_quantum_left = TIME_QUANTUM;
    }
    
    return next;
}
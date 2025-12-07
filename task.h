// task.h
#ifndef TASK_H
#define TASK_H

// 任務狀態
typedef enum {
    READY,
    RUNNING,
    SLEEPING,
    TERMINATED
} TaskState;

// 排程演算法類型
typedef enum {
    SCHED_FCFS,
    SCHED_RR,
    SCHED_SJF,
    SCHED_PRIORITY,
    SCHED_LIFO // 新增 LIFO 類型
} SchedulerType;

// 任務控制塊 (PCB - Process Control Block)
typedef struct Task {
    int id;
    char name[20];
    
    // 排程相關參數
    int burst_time;       // 總執行時間
    int remaining_time;   // 剩餘執行時間
    int priority;         // 優先權 (數值大 = 優先權高)
    
    // 執行狀態
    TaskState state;
    int wake_up_time;     // 預計喚醒時間 (用於 Sleep)
    int rr_quantum_left;  // RR 剩餘時間片
    
    struct Task* next;    // 連結串列指標
} Task;

// 建立新任務的輔助函式 (實作在 queue.c 或 main.c 皆可，這裡我們放在 queue.c 方便管理)
Task* createTask(int id, const char* name, int burst, int prio);

#endif
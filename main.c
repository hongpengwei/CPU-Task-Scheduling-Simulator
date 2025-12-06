// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "queue.h"
#include "scheduler.h"
#include "system.h"

int main(int argc, char* argv[]) {
    // 1. 處理參數
    if (argc < 2) {
        printf("Usage: %s [FCFS|RR|SJF|PRIORITY]\n", argv[0]);
        return 1;
    }

    // 設定排程器
    if (strcmp(argv[1], "FCFS") == 0) SetSchedulerType(SCHED_FCFS);
    else if (strcmp(argv[1], "RR") == 0) SetSchedulerType(SCHED_RR);
    else if (strcmp(argv[1], "SJF") == 0) SetSchedulerType(SCHED_SJF);
    else if (strcmp(argv[1], "PRIORITY") == 0) SetSchedulerType(SCHED_PRIORITY);
    else {
        printf("Invalid scheduler type.\n");
        return 1;
    }

    SystemInit();
    printf("Simulation Starting with %s Scheduler...\n", argv[1]);

    // 2. 建立測試任務 (ID, Name, Burst, Priority)
    // 這裡我們直接呼叫 AddToReadyQueue 讓它們進入系統
    AddToReadyQueue(createTask(1, "Task_A", 6, 1));
    AddToReadyQueue(createTask(2, "Task_B", 10, 3)); // Priority High
    AddToReadyQueue(createTask(3, "Task_C", 4, 2));

    Task* current_task = NULL;
    int tasks_done = 0;
    const int TOTAL_TASKS = 3;

    // 3. 模擬主迴圈
    while (tasks_done < TOTAL_TASKS) {
        printf("\nTick %d:\n", GLOBAL_TIME);

        // A. 硬體中斷：檢查鬧鐘
        CheckAlarm();

        // B. 排程決策：如果 CPU 閒置，選新任務
        if (current_task == NULL) {
            current_task = GetNextTask();
            if (current_task != NULL) {
                current_task->state = RUNNING;
                printf("  -> Context Switch: %s started running.\n", current_task->name);
            }
        }

        // C. 執行與模擬 User Code
        if (current_task != NULL) {
            
            // --- 模擬 User Program 行為 ---
            // 模擬：Task_B 執行到剩 8 ticks 時，呼叫 Sleep(5)
            if (strcmp(current_task->name, "Task_B") == 0 && current_task->remaining_time == 8) {
                SysCall_Sleep(current_task, 5);
                current_task = NULL; // 讓出 CPU
            } 
            else {
                // 正常 CPU 執行
                printf("  [CPU] Executing %s (Remaining: %d)\n", current_task->name, current_task->remaining_time);
                
                current_task->remaining_time--;
                if (GetSchedulerType() == SCHED_RR) {
                    current_task->rr_quantum_left--;
                }

                // 任務完成
                if (current_task->remaining_time == 0) {
                    printf("  *** %s Finished! ***\n", current_task->name);
                    current_task->state = TERMINATED;
                    free(current_task);
                    current_task = NULL;
                    tasks_done++;
                }
                // RR Time Quantum 用完 (Preemption)
                else if (GetSchedulerType() == SCHED_RR && current_task->rr_quantum_left == 0) {
                    printf("  -> RR Time Quantum expired for %s.\n", current_task->name);
                    AddToReadyQueue(current_task); // 放回隊伍
                    current_task = NULL; // 讓出 CPU
                }
            }
        } else {
            printf("  [IDLE] CPU is idle.\n");
        }

        GLOBAL_TIME++;
        if (GLOBAL_TIME > 100) break; // 安全中止
    }

    printf("\nSimulation Finished.\n");
    return 0;
}
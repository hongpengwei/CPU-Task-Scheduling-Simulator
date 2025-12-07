// main.c (Modified)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "queue.h"
#include "scheduler.h"
#include "system.h"

int main(int argc, char* argv[]) {
    // 1. 處理參數並設定排程器
    if (argc < 2) {
        printf("Usage: %s [FCFS|RR|SJF|PRIORITY]\n", argv[0]);
        return 1;
    }

    // 設定排程器
    if (strcmp(argv[1], "FCFS") == 0) SetSchedulerType(SCHED_FCFS);
    else if (strcmp(argv[1], "RR") == 0) SetSchedulerType(SCHED_RR);
    else if (strcmp(argv[1], "SJF") == 0) SetSchedulerType(SCHED_SJF);
    else if (strcmp(argv[1], "PRIORITY") == 0) SetSchedulerType(SCHED_PRIORITY);
    // --- 新增 LIFO 處理 ---
    else if (strcmp(argv[1], "LIFO") == 0) SetSchedulerType(SCHED_LIFO);
    // ----------------------
    else {
        // 也要更新提示訊息
        printf("Invalid scheduler type. Must be FCFS, RR, SJF, PRIORITY, or LIFO.\n");
        return 1;
    }

    SystemInit();
    printf("Simulation Starting with %s Scheduler...\n", argv[1]);
    
    // --- 新增: 2. 讓使用者輸入任務資訊 ---
    int total_tasks = 0;
    printf("\nPlease enter the total number of tasks: ");
    if (scanf("%d", &total_tasks) != 1 || total_tasks <= 0) {
        printf("Invalid task count.\n");
        return 1;
    }

    char task_name[20];
    int burst, priority;

    for (int i = 1; i <= total_tasks; i++) {
        snprintf(task_name, sizeof(task_name), "Task_%d", i);
        
        printf("  Enter Burst Time for %s: ", task_name);
        if (scanf("%d", &burst) != 1 || burst <= 0) {
            printf("Invalid burst time. Exiting.\n");
            return 1;
        }

        // 只有在 PRIORITY 模式才要求輸入優先權，否則設為預設值
        if (GetSchedulerType() == SCHED_PRIORITY) {
            printf("  Enter Priority (Higher number = Higher Priority) for %s: ", task_name);
            if (scanf("%d", &priority) != 1) {
                 printf("Invalid priority. Exiting.\n");
                return 1;
            }
        } else {
            priority = 0; // 對於非優先權排程器，優先權不重要
        }
        
        // 創建任務並加入 ReadyQueue
        AddToReadyQueue(createTask(i, task_name, burst, priority));
    }
    printf("All tasks loaded. Starting simulation loop.\n");
    // --- 任務輸入結束 ---


    Task* current_task = NULL;
    int tasks_done = 0;
    const int TOTAL_TASKS = total_tasks; // 使用者輸入的任務總數

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
                printf("  -> Context Switch: %s started running (Remaining: %d).\n", 
                       current_task->name, current_task->remaining_time);
            }
        }

        // C. 執行與模擬 User Code
        if (current_task != NULL) {
            
            // *** 移除 Task_B 的硬編碼 SysCall_Sleep 模擬，保持模擬的通用性 ***
            // *** 如果需要 Sleep 模擬，應該改成通用邏輯或使用者參數控制 ***
            
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
        } else {
            printf("  [IDLE] CPU is idle. Checking Sleep List...\n");
        }

        GLOBAL_TIME++;
        if (GLOBAL_TIME > 1000) { // 提高安全中止時間
            printf("\nSafety limit reached. Stopping simulation.\n");
            break; 
        }
    }

    printf("\nSimulation Finished. Total Ticks: %d\n", GLOBAL_TIME - 1);
    return 0;
}
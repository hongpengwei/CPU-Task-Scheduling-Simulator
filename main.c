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
        printf("Usage: %s [FCFS|RR|SJF|PRIORITY|LIFO]\n", argv[0]);
        return 1;
    }

    // 設定排程器
    if (strcmp(argv[1], "FCFS") == 0) SetSchedulerType(SCHED_FCFS);
    else if (strcmp(argv[1], "RR") == 0) SetSchedulerType(SCHED_RR);
    else if (strcmp(argv[1], "SJF") == 0) SetSchedulerType(SCHED_SJF);
    else if (strcmp(argv[1], "PRIORITY") == 0) SetSchedulerType(SCHED_PRIORITY);
    else if (strcmp(argv[1], "LIFO") == 0) SetSchedulerType(SCHED_LIFO); // 新增 LIFO
    else {
        printf("Invalid scheduler type. Must be FCFS, RR, SJF, PRIORITY, or LIFO.\n");
        return 1;
    }

    SystemInit();
    printf("Simulation Starting with %s Scheduler...\n", argv[1]);
    
    // --- 2. 讓使用者輸入任務資訊 (含 I/O) ---
    int total_tasks = 0;
    printf("\nPlease enter the total number of tasks: ");
    if (scanf("%d", &total_tasks) != 1 || total_tasks <= 0) {
        printf("Invalid task count.\n");
        return 1;
    }

    char task_name[20];
    int burst, priority;
    int io_cycle, io_sleep; // NEW: I/O 參數

    for (int i = 1; i <= total_tasks; i++) {
        snprintf(task_name, sizeof(task_name), "Task_%d", i);
        
        printf("  Enter Burst Time for %s: ", task_name);
        if (scanf("%d", &burst) != 1 || burst <= 0) {
            printf("Invalid burst time. Exiting.\n");
            return 1;
        }

        // 輸入 I/O 週期
        printf("  Enter I/O Burst Cycle (Ticks before I/O, 0 if none): ");
        if (scanf("%d", &io_cycle) != 1) { printf("Invalid cycle. Exiting.\n"); return 1; }

        if (io_cycle > 0) {
            printf("  Enter I/O Sleep Duration: ");
            if (scanf("%d", &io_sleep) != 1 || io_sleep <= 0) {
                printf("Invalid sleep duration. Exiting.\n");
                return 1;
            }
        } else {
            io_sleep = 0;
        }

        // 輸入 Priority
        if (GetSchedulerType() == SCHED_PRIORITY) {
            printf("  Enter Priority (Higher number = Higher Priority) for %s: ", task_name);
            if (scanf("%d", &priority) != 1) {
                 printf("Invalid priority. Exiting.\n");
                return 1;
            }
        } else {
            priority = 0;
        }
        
        // 創建任務時傳入 I/O 參數
        AddToReadyQueue(createTask(i, task_name, burst, priority, io_cycle, io_sleep));
    }
    printf("All tasks loaded. Starting simulation loop.\n");
    // --- 任務輸入結束 ---


    Task* current_task = NULL;
    int tasks_done = 0;
    const int TOTAL_TASKS = total_tasks; 

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

        // C. 執行與 I/O 檢查
        if (current_task != NULL) {
            
            // 1. 正常 CPU 執行
            printf("  [CPU] Executing %s (Remaining: %d)\n", current_task->name, current_task->remaining_time);
            
            current_task->remaining_time--;

            // 2. 更新 I/O 週期計數器 (如果任務有 I/O)
            if (current_task->io_burst_cycle > 0) {
                current_task->current_cycle_count++;
            }
            
            // 3. RR Time Quantum 檢查
            if (GetSchedulerType() == SCHED_RR) {
                current_task->rr_quantum_left--;
            }

            // 4. 通用 I/O 檢查: 週期計數器達到 I/O 週期，且任務尚未完成
            if (current_task->io_burst_cycle > 0 && 
                current_task->current_cycle_count == current_task->io_burst_cycle && 
                current_task->remaining_time > 0) 
            {
                // 執行 SysCall_Sleep
                SysCall_Sleep(current_task, current_task->io_sleep_duration);
                current_task->current_cycle_count = 0; // 重置週期計數
                current_task = NULL; // 讓出 CPU
            }
            // 5. 任務完成
            else if (current_task != NULL && current_task->remaining_time == 0) {
                printf("  *** %s Finished! ***\n", current_task->name);
                current_task->state = TERMINATED;
                free(current_task);
                current_task = NULL;
                tasks_done++;
            }
            // 6. RR Time Quantum 用完 (Preemption)
            else if (GetSchedulerType() == SCHED_RR && current_task != NULL && current_task->rr_quantum_left == 0) {
                printf("  -> RR Time Quantum expired for %s.\n", current_task->name);
                AddToReadyQueue(current_task); // 放回隊伍
                current_task = NULL; // 讓出 CPU
            }
        } else {
            printf("  [IDLE] CPU is idle.\n");
        }

        GLOBAL_TIME++;
        if (GLOBAL_TIME > 1000) { 
            printf("\nSafety limit reached. Stopping simulation.\n");
            break; 
        }
    }

    printf("\nSimulation Finished. Total Ticks: %d\n", GLOBAL_TIME - 1);
    return 0;
}
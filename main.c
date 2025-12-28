#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "queue.h"
#include "scheduler.h"
#include "system.h"

// 設定 Context Switch 的成本 (Ticks)
#define CONTEXT_SWITCH_COST 1 

// 輔助函式：更新等待時間與老化機制
void UpdateQueueStats() {
    Task* t = ReadyQueue.head;
    while (t != NULL) {
        // 1. 所有在 ReadyQueue 的人，等待時間 +1
        t->total_waiting_time++;

        // 2. Aging 機制：如果是 Priority 排程，且每經過 10 ticks，優先權 +1
        // (這能避免低優先權任務永遠跑不到)
        if (GetSchedulerType() == SCHED_PRIORITY && (GLOBAL_TIME % 10 == 0)) {
            t->priority++;
            // 注意：為了效能，這裡只改數值。
            // 若要嚴格排序，通常需將任務移出再重新插入，但在模擬器中這樣顯示數值變化即可。
            printf("    [Aging] Task %s priority increased to %d\n", t->name, t->priority);
        }

        t = t->next;
    }
}

int main(int argc, char* argv[]) {
    // 1. 處理參數並設定排程器
    if (argc < 2) {
        printf("Usage: %s [FCFS|RR|SJF|PRIORITY|LIFO]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "FCFS") == 0) SetSchedulerType(SCHED_FCFS);
    else if (strcmp(argv[1], "RR") == 0) SetSchedulerType(SCHED_RR);
    else if (strcmp(argv[1], "SJF") == 0) SetSchedulerType(SCHED_SJF);
    else if (strcmp(argv[1], "PRIORITY") == 0) SetSchedulerType(SCHED_PRIORITY);
    else if (strcmp(argv[1], "LIFO") == 0) SetSchedulerType(SCHED_LIFO);
    else {
        printf("Invalid scheduler type.\n");
        return 1;
    }

    SystemInit();
    printf("Simulation Starting with %s Scheduler...\n", argv[1]);
    
    // --- 2. 輸入任務資訊 ---
    int total_tasks = 0;
    printf("\nPlease enter the total number of tasks: ");
    if (scanf("%d", &total_tasks) != 1 || total_tasks <= 0) return 1;

    char task_name[20];
    int burst, priority, io_cycle, io_sleep;

    for (int i = 1; i <= total_tasks; i++) {
        snprintf(task_name, sizeof(task_name), "Task_%d", i);
        printf("  Enter Burst Time for %s: ", task_name);
        scanf("%d", &burst);
        printf("  Enter I/O Burst Cycle (0 if none): ");
        scanf("%d", &io_cycle);
        io_sleep = (io_cycle > 0) ? 0 : 0;
        if (io_cycle > 0) {
            printf("  Enter I/O Sleep Duration: ");
            scanf("%d", &io_sleep);
        }
        
        priority = 0;
        if (GetSchedulerType() == SCHED_PRIORITY) {
            printf("  Enter Priority (Higher = Higher Priority): ");
            scanf("%d", &priority);
        }
        
        AddToReadyQueue(createTask(i, task_name, burst, priority, io_cycle, io_sleep));
    }
    printf("All tasks loaded. Starting simulation loop.\n");

    Task* current_task = NULL;
    int tasks_done = 0;
    const int TOTAL_TASKS = total_tasks; 
    
    // 用於模擬 Context Switch 的計數器
    int cs_counter = 0; 
    Task* next_task_candidate = NULL;

    // 3. 模擬主迴圈
    while (tasks_done < TOTAL_TASKS) {
        printf("\nTick %d:\n", GLOBAL_TIME);

        // A. 硬體中斷：檢查鬧鐘 (喚醒睡覺的行程)
        CheckAlarm();

        // --- B. 排程與 Context Switch 模擬 ---
        if (current_task == NULL) {
            // 如果還沒選定下一個候選人，且正在切換中
            if (cs_counter > 0) {
                cs_counter--;
                printf("  [System] Context Switching... (Overhead)\n");
                
                // 若切換時間結束，正式讓新任務上線
                if (cs_counter == 0 && next_task_candidate != NULL) {
                    current_task = next_task_candidate;
                    current_task->state = RUNNING;
                    next_task_candidate = NULL;
                    printf("  -> Switch Complete: %s started running.\n", current_task->name);
                }
            } 
            // 如果 CPU 閒置且沒在切換，嘗試抓新任務
            else {
                if (next_task_candidate == NULL) {
                    next_task_candidate = GetNextTask();
                }

                if (next_task_candidate != NULL) {
                    // 觸發 Context Switch
                    cs_counter = CONTEXT_SWITCH_COST;
                    printf("  [System] Context Switch Triggered (Cost: %d ticks). preparing %s\n", 
                           CONTEXT_SWITCH_COST, next_task_candidate->name);
                    
                    // 如果 Cost 設為 0，直接執行 (避免邏輯卡住)
                    if (cs_counter == 0) {
                        current_task = next_task_candidate;
                        current_task->state = RUNNING;
                        next_task_candidate = NULL;
                    }
                }
            }
        }

        // C. 更新等待時間與老化 (關鍵修改)
        UpdateQueueStats();

        // D. 執行任務邏輯 (只有當 CPU 真的有任務且沒在切換時)
        if (current_task != NULL && cs_counter == 0) {
            
            printf("  [CPU] Executing %s (Remaining: %d)\n", current_task->name, current_task->remaining_time);
            
            current_task->remaining_time--;
            
            // I/O 邏輯
            if (current_task->io_burst_cycle > 0) current_task->current_cycle_count++;
            
            // RR Time Quantum 邏輯
            if (GetSchedulerType() == SCHED_RR) current_task->rr_quantum_left--;

            // 檢查 I/O 觸發
            if (current_task->io_burst_cycle > 0 && 
                current_task->current_cycle_count == current_task->io_burst_cycle && 
                current_task->remaining_time > 0) 
            {
                SysCall_Sleep(current_task, current_task->io_sleep_duration);
                current_task->current_cycle_count = 0;
                current_task = NULL; // 讓出 CPU
            }
            // 檢查任務完成
            else if (current_task->remaining_time == 0) {
                printf("  *** %s Finished! ***\n", current_task->name);
                printf("      Stats -> Total Waiting Time: %d ticks\n", current_task->total_waiting_time);
                
                current_task->state = TERMINATED;
                free(current_task);
                current_task = NULL;
                tasks_done++;
            }
            // 檢查 RR Time Quantum
            else if (GetSchedulerType() == SCHED_RR && current_task->rr_quantum_left == 0) {
                printf("  -> RR Time Quantum expired for %s.\n", current_task->name);
                AddToReadyQueue(current_task);
                current_task = NULL; // 讓出 CPU
            }
        } else if (current_task == NULL && cs_counter == 0) {
            printf("  [IDLE] CPU is idle.\n");
        }

        GLOBAL_TIME++;
        if (GLOBAL_TIME > 1000) break; // 安全中止
    }

    printf("\nSimulation Finished. Total Ticks: %d\n", GLOBAL_TIME - 1);
    return 0;
}
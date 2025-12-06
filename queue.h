// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "task.h"

// 佇列結構
typedef struct {
    Task* head;
    Task* tail;
} Queue;

// 初始化
void initQueue(Queue* q);

// 基本操作
void enqueue(Queue* q, Task* t);
Task* dequeue(Queue* q);
int isEmpty(Queue* q);

// 關鍵演算法：有序插入
// mode: 0=WakeTime(Asc), 1=Remaining(Asc), 2=Priority(Desc)
void insertSorted(Queue* q, Task* t, int mode);

#endif
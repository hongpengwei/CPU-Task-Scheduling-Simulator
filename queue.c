// queue.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void initQueue(Queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

Task* createTask(int id, const char* name, int burst, int prio) {
    Task* t = (Task*)malloc(sizeof(Task));
    t->id = id;
    strcpy(t->name, name);
    t->burst_time = burst;
    t->remaining_time = burst;
    t->priority = prio;
    t->state = READY;
    t->wake_up_time = 0;
    t->rr_quantum_left = 0;
    t->next = NULL;
    return t;
}

void enqueue(Queue* q, Task* t) {
    t->next = NULL;
    if (q->head == NULL) {
        q->head = t;
        q->tail = t;
    } else {
        q->tail->next = t;
        q->tail = t;
    }
}

Task* dequeue(Queue* q) {
    if (q->head == NULL) return NULL;
    Task* t = q->head;
    q->head = q->head->next;
    if (q->head == NULL) q->tail = NULL;
    t->next = NULL;
    return t;
}

int isEmpty(Queue* q) {
    return q->head == NULL;
}

// 實作多型插入邏輯
void insertSorted(Queue* q, Task* t, int mode) {
    Task* current;
    int head_condition = 0;

    // 判斷是否需要插在頭部
    if (q->head == NULL) {
        head_condition = 1;
    } else {
        if (mode == 0)      head_condition = (t->wake_up_time < q->head->wake_up_time);
        else if (mode == 1) head_condition = (t->remaining_time < q->head->remaining_time);
        else if (mode == 2) head_condition = (t->priority > q->head->priority);
    }

    if (head_condition) {
        t->next = q->head;
        q->head = t;
        if (q->tail == NULL) q->tail = t;
    } else {
        current = q->head;
        while (current->next != NULL) {
            int condition = 0;
            if (mode == 0)      condition = (t->wake_up_time < current->next->wake_up_time);
            else if (mode == 1) condition = (t->remaining_time < current->next->remaining_time);
            else if (mode == 2) condition = (t->priority > current->next->priority);
            
            if (condition) break;
            current = current->next;
        }
        t->next = current->next;
        current->next = t;
        if (t->next == NULL) q->tail = t;
    }
}
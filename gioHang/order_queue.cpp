#include "gioHang.h"

void initQueue(OrderQueue& q) {
    q.front = nullptr;
    q.rear = nullptr;
}

bool isEmptyQueue(OrderQueue& q) {
    return q.front == nullptr;
}

void enqueueOrder(OrderQueue& q, Order val) {
    QueueNode* newNode = new QueueNode{val, nullptr};
    if (isEmptyQueue(q)) {
        q.front = newNode;
    } else {
        q.rear->next = newNode;
    }
    q.rear = newNode;
}

Order dequeueOrder(OrderQueue& q) {
    QueueNode* tmp = q.front;
    Order data = tmp->data;
    q.front = q.front->next;
    if (q.front == nullptr) {
        q.rear = nullptr;
    }
    delete tmp;
    return data;
}

void destroyQueue(OrderQueue& q) {
    while (!isEmptyQueue(q)) {
        dequeueOrder(q);
    }
}
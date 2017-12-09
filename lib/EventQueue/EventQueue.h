#include "Arduino.h"

typedef struct qItem {
    struct qItem *next;
      void *value;
} QueueItem;

typedef struct queue {
    QueueItem *first;
    QueueItem *last;
} Queue;

Queue *createQueue();
void destroyQueue(Queue *queue);
unsigned char empty(Queue *queue);
void enqueue(Queue *queue, void *value);
int dequeue(Queue *queue, void **value);

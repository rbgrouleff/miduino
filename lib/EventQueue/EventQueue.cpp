#include <stdlib.h>
#include "EventQueue.h"

unsigned char empty(Queue *queue)
{
  return queue->first == NULL;
}

Queue *createQueue()
{
  Queue *queue = (Queue *) malloc(sizeof(Queue));
  queue->first = NULL;
  queue->last = NULL;
  return queue;
}

void destroyQueue(Queue *queue)
{
  while (!empty(queue))
  {
    void **_;
    dequeue(queue, _);
  }
  free(queue);
}

void enqueue(Queue *queue, void *value)
{
  QueueItem *item = (QueueItem *) malloc(sizeof(QueueItem));
  item->value = value;
  item->next = NULL;
  if (empty(queue))
  {
    queue->first = item;
  }
  else
  {
    QueueItem *last = queue->last;
    last->next = item;
  }
  queue->last = item;
}


int dequeue(Queue *queue, void **value)
{
  QueueItem *item = queue->first;
  int result;
  if (item)
  {
    QueueItem *next = item->next;
    *value = item->value;
    queue->first = next;
    if (next == NULL)
    {
      queue->last = NULL;
    }
    free(item);
    result = 0;
  }
  else
  {
    result = -1;
  }
  return result;
}

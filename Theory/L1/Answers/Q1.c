#define MAX_QUEUE_LEN 20
#include <stdlib.h>

typedef struct Queue
{
  int arr[MAX_QUEUE_LEN];

  int head;
  int tail;
} Queue;

int is_empty(Queue *q)
{
  return q->head == -1 && q->tail == -1;
}

int is_full(Queue *q)
{
  return q->tail == MAX_QUEUE_LEN - 1;
  // Circular: ((q->tail + 1) % MAX_QUEUE_LEN) == q->head;
}

void enqueue(Queue *q, int value)
{
  if (is_full(q))
    return;

  if (is_empty(q))
    q->head = q->tail = 0;
  else
    q->tail++;
  // Circular: (q->tail + 1) % MAX_QUEUE_LEN;

  q->arr[q->tail] = value;
}

int dequeue(Queue *q)
{
  if (is_empty(q))
    return -1;

  int value = q->arr[q->head];

  if (q->head == q->tail)
  {
    q->head = q->tail = -1;
  }
  else
  {
    q->head++;
    /*
    Circular: (q->head + 1) % MAX_QUEUE_LEN;
    */
  }

  return value;
}
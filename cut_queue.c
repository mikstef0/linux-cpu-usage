#include "cut_queue.h"

void dqueue_init(dq* queue)
{
    queue->q_front = -1;
    queue->q_rear = -1;
}

int dqueue_empty(dq* queue)
{
    return (queue->q_front == -1 && queue->q_rear == -1);
}

int dqueue_full(dq* queue)
{
    return ((queue->q_rear + 1) % MAX_Q_SIZE == queue->q_front);
}

void dqueue_enq(dq* queue, struct procdata_struct item)
{
    if (dqueue_full(queue))
    {
        fprintf(stderr, "DQueue is full. Cannot enqueue.\n");
        return;
    }

    if (dqueue_empty(queue))
    {
        queue->q_front = 0;
        queue->q_rear = 0;
    }
    else
    {
        queue->q_rear = (queue->q_rear + 1) % MAX_Q_SIZE;
    }

    strcpy(queue->data[queue->q_rear].cpuno, item.cpuno);
    for(int i=0; i<10; i++)
    {
        queue->data[queue->q_rear].values[i]=item.values[i];
    }
}

struct procdata_struct dqueue_deq(dq* queue)
{
    if (dqueue_empty(queue))
    {
        fprintf(stderr, "DQueue is empty. Cannot dequeue.\n");
    }

    struct procdata_struct item;
    strcpy(item.cpuno, queue->data[queue->q_front].cpuno);
    for(int i=0; i<10; i++)
    {
        item.values[i]=queue->data[queue->q_front].values[i];
    }

    if (queue->q_front == queue->q_rear)
    {
        queue->q_front = -1;
        queue->q_rear = -1;
    }
    else
    {
        queue->q_front = (queue->q_front + 1) % MAX_Q_SIZE;
    }

    return item;
}


void pqueue_init(pq* queue)
{
    queue->q_front = -1;
    queue->q_rear = -1;
}

int pqueue_empty(pq* queue)
{
    return (queue->q_front == -1 && queue->q_rear == -1);
}

int pqueue_full(pq* queue)
{
    return ((queue->q_rear + 1) % MAX_Q_SIZE == queue->q_front);
}

void pqueue_enq(pq* queue, struct procdata_struct item)
{
    if (pqueue_full(queue))
    {
        fprintf(stderr, "PQueue is full. Cannot enqueue.\n");
        return;
    }

    if (pqueue_empty(queue))
    {
        queue->q_front = 0;
        queue->q_rear = 0;
    }
    else
    {
        queue->q_rear = (queue->q_rear + 1) % MAX_Q_SIZE;
    }

    strcpy(queue->data[queue->q_rear].cpuno, item.cpuno);
    for(int i=0; i<10; i++)
    {
        queue->data[queue->q_rear].values[i]=item.values[i];
    }
}

struct procdata_struct pqueue_deq(pq* queue)
{
    if (pqueue_empty(queue))
    {
        fprintf(stderr, "PQueue is empty. Cannot dequeue.\n");
    }

    struct procdata_struct item;
    strcpy(item.cpuno, queue->data[queue->q_front].cpuno);
    for(int i=0; i<10; i++)
    {
        item.values[i]=queue->data[queue->q_front].values[i];
    }

    if (queue->q_front == queue->q_rear)
    {
        queue->q_front = -1;
        queue->q_rear = -1;
    }
    else
    {
        queue->q_front = (queue->q_front + 1) % MAX_Q_SIZE;
    }

    return item;
}





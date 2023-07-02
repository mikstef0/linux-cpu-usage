#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

void dqueue_init(dq* queue); // initialise /proc/stat data queue
int dqueue_empty(dq* queue); // check whether /proc/stat data queue is empty
int dqueue_full(dq* queue); // chech whether /proc/stat data queue is full
void dqueue_enq(dq* queue, struct procdata_struct item); // add an element to /proc/stat data queue
struct procdata_struct dqueue_deq(dq* queue); // take front element from /proc/stat data queue

void pqueue_init(pq* queue); // initialise CPU usage percentages queue
int pqueue_empty(pq* queue); // check whether CPU usage percentages queue is empty
int pqueue_full(pq* queue); // chech whether CPU usage percentages queue is full
void pqueue_enq(pq* queue, struct procdata_struct item); // add an element to CPU usage percentages queue
struct procdata_struct pqueue_deq(pq* queue); // take front element from CPU usage percentages queue

#endif
 

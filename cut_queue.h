#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "global.h"

void dqueue_init(dq* queue); // initialise /proc/stat data queue
int dqueue_empty(dq* queue); // check whether /proc/stat data queue is empty
int dqueue_full(dq* queue); // chech whether /proc/stat data queue is full
void dqueue_enq(dq* queue, struct procdata_struct item); // add an element to /proc/stat data queue
struct procdata_struct dqueue_deq(dq* queue); // take front element from /proc/stat data queue

#endif


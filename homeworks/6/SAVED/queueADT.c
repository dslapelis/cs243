/// author: daniel slapelis
/// id: djs7209
/// date: 04/21/2015
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct QueueADT
{
    int front;
    int rear;
    int size;
    void **queuearr;
    int (*compare)(const void*a, const void*b);
};

typedef struct QueueADT *QueueADT;
#define _QUEUE_IMPL_
#include "queueADT.h"

/// declaring a push method
void push(QueueADT queue, void* data);

void* pop(QueueADT queue);

QueueADT que_create( int (*cmp)(const void *a, const void *b) )
{
    QueueADT queue = malloc(sizeof(QueueADT));
    queue->compare = cmp;
    void **queuearr = malloc(sizeof(long));
    /// our queue
    queue->queuearr = queuearr;
    /// where to push onto queue
    queue->front = 0;
    /// where to pop queue from
    queue->rear = -1;
    /// number of elements in queue
    queue->size = 0;
    return queue;
}

void que_destroy( QueueADT queue )
{
    free(queue->queuearr);
    free(queue);
}

void que_clear( QueueADT queue )
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    free(queue->compare);
    free(queue->queuearr);
}

void que_insert( QueueADT queue, void *data )
{
    if(queue == NULL)
    {
        exit(EXIT_FAILURE);
    }
    /// if the queue is empty, just put this element here
    if(queue->size == 0)
    {
        push(queue,data);
    }else{
        if(queue->compare != NULL) /// so we dont segfault when no compare is given
        {
            /// we insert based on the compare algorithm
            int it = queue->size;
            for(int i = 0; i < it; i++)
            {
                void* curr = pop(queue);
                if(queue->compare(data, curr) > 0)
                {
                    push(queue, curr);
                }else{
                    push(queue, data);
                    data = curr;
                }
             }
             push(queue, data);
        }else{
            push(queue, data);
        }
    } 
}

void *que_remove( QueueADT queue )
{
    /// FIFO
    void *val = pop(queue);
    return val;
}

bool que_empty( QueueADT queue )
{
    if(queue->rear < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/// push function to manipulate array like a queue
/// we create a new array of size+1. from here we copy
/// our data value into position 0 and then put the values
/// from the old array into position i+1 until they're all
/// added. 
void push(QueueADT queue, void* data)
{
    void **newqueue = malloc(sizeof(long) * (queue->size+1));
    for(int i = 0; i < queue->size+1; i++)
    {
        newqueue[i] = NULL;
    }

    newqueue[0] = data;

    for(int i = 0; i < queue->size; i++)
    {
        newqueue[i+1] = queue->queuearr[i];
        queue->queuearr[i] = NULL;
    }  

    free(queue->queuearr);
    queue->queuearr = newqueue;
    queue->size++;
    queue->rear++;
}

/// pop function to manipulate array like a queue
/// we create a new array of size-1. from here we save the 
/// value at [rear] of our previous array. then we add the values
/// into this array from the old one, starting from position 0.
/// we free the old array and point the queue to this one, then we
///return the popped value
void* pop(QueueADT queue)
{
    void** newqueue = malloc(sizeof(long) * (queue->size-1));
    void* popelement = queue->queuearr[queue->rear];
    queue->queuearr[queue->rear] = NULL;
    for(int i = 0; i < queue->rear; i++)
    {
        newqueue[i] = queue->queuearr[i];
        queue->queuearr[i] = NULL;
    }

    free(queue->queuearr);
    queue->queuearr = newqueue;
    queue->size--;
    queue->rear--;
    return popelement;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//
// Only define the QueueADT type if we are _not_ compiling the
// module's implementation file.
//
// In the implementation file, define the QueueADT type as you need
// to for the implementation, and then define the symbol _QUEUE_IMPL_
// immediately before including this header file to prevent a
// redefinition of the type.
//
struct QueueADT
{
    int front;
    int rear;
    int size;
    int capacity;
    void **queuearr;
    int (*compare)(const void *a, const void *b);
};

typedef struct QueueADT *QueueADT;
#define _QUEUE_IMPL_
#include "queueADT.h"

/// declaring push and pop
void push(QueueADT queue, void* data);
void* pop(QueueADT queue);

/// Create a QueueADT which uses the supplied function as a comparison
/// routine.
///
/// The comparison function takes two void* parameters, and returns
/// an integer result which indicates the relationship between the
/// two things:
///
///     Result  Relationship
///     ======  ===========
///      < 0    a < b
///      = 0    a == b
///      > 0    a > b
///
/// where ">" and "<" are dependent upon the data being compared
///
/// @param cmp the address of the comparison function to be used for
///    ordering this queue, or NULL if standard FIFO behavior is desired
/// @return a pointer to a QueueADT, or NULL if the allocation fails/// Create a QueueADT which uses the supplied function as a comparisonoutine.

QueueADT que_create( int (*cmp)(const void *a, const void *b) )
{
    QueueADT queue = malloc(sizeof(QueueADT)*10);
    queue->compare = cmp;
    void **queuearr = malloc(sizeof(long)*10); /// init to 10 positions
    queue->queuearr = queuearr;
    queue->front = 0; /// uninitialized
    queue->rear = -1; /// uninitialized
    queue->size = 0; /// num of elements in queue
    queue->capacity = 10; /// can hold 10 elements
   return queue;
}
/// Tear down and deallocate the supplied QueuADT.
///
/// @param queue - the QueueADT to be manipulated
void que_destroy( QueueADT queue )
{
    free(queue->queuearr);
    free(queue);
}

/// Remove all contents from the supplied QueueADT.
///
/// @param queue - the QueueADT to be manipuated
void que_clear( QueueADT queue )
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    free(queue->queuearr);
}

/// Insert the specified data into the Queue in the appropriate place
///
/// Uses the queue's comparison function to determine the appropriate
/// place for the insertion.
///
/// @param queue the QueueADT into which the value is to be inserted
/// @param data the data to be inserted
/// @exception If the value cannot be inserted into the queue for
///     whatever reason (usually implementation-dependent), the program
///     should terminate with an error message.  This can be done by
///     printing an appropriate message to the standard error output and
///     then exiting with EXIT_FAILURE, or by having an assert() fail.
void que_insert( QueueADT queue, void *data )
{
    if(queue == NULL)
    {
        exit(EXIT_FAILURE);
    }
    
    if(queue->size == 0) /// empty queue, put element at 0
    {
        push(queue, data);
    }else{
        if(queue->compare != NULL)
        {
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

/// Remove and return the first element from the QueueADT.
///
/// @param queue the QueueADT to be manipulated
/// @return the value that was removed from the queue
/// @exception If the queue is empty, the program should terminate
///     with an error message.  This can be done by printing an
///     appropriate message to the standard error output and then
///     exiting with EXIT_FAILURE, or by having an assert() fail.
void* que_remove( QueueADT queue )
{
    void *val = pop(queue);
    return val;
}

/// Indicate whether or not the supplied Queue is empty
///
/// @param the QueueADT to be tested
/// @return true if the queue is empty, otherwise false
bool que_empty( QueueADT queue )
{
    if(queue->size == 0)
    {
        return true;
    }else{
        return false;
    }
}

/// pushes data onto the rear of the array
/// if queue->rear is approaching queue->capacity
/// we double the queue's capacity and place all the elements
/// in it and continue like normal. remade the whole push and pop
/// functions to properly manipulate the array like a queue without
/// taking an eternity
void push(QueueADT queue, void* data)
{

    /// if our queue is almost full we resize it
    if(queue->rear == queue->capacity-1)
    {
        void **newqueue = malloc(sizeof(long) * (queue->capacity * 2));
        for(int i = 0; i < queue->capacity; i++)
        {
            newqueue[i] = queue->queuearr[i];
        }
        queue->capacity = queue->capacity * 2;
        free(queue->queuearr);
        queue->queuearr = newqueue;
    }
        /// place our data into the rear
        queue->rear++;
        queue->size++;
        queue->queuearr[queue->rear] = data;
}

/// removes elements from the front of the array and decrements it properly
/// only problem is that the queue goes on to infinity as we remove and add elements...
/// its like a train lol
void* pop(QueueADT queue)
{
    void* popped = queue->queuearr[queue->front];
    queue->front++;
    queue->size--;
    return popped;
}

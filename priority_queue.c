#include "priority_queue.h"
#include <assert.h>
#include <stdlib.h>

#define NULL_OBJECT -1

struct PriorityQueue_t {
    PQElement pq_element;
    PQElementPriority pq_element_priority;
    int elemnts_counter;
    struct PriorityQueue_t* next;

    /* Elemetn operations */ 
    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;

    /* priority operations */
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priorities;
};


PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities)
{
    if(!copy_element || !free_element || !equal_elements || !copy_priority || !free_priority || !compare_priorities)
    {
        return NULL;
    }

    PriorityQueue priority_queue = malloc(sizeof(*priority_queue));
    if(priority_queue == NULL)
    {
        return NULL;
    }

    priority_queue->elemnts_counter = 0;
    priority_queue->pq_element = NULL;
    priority_queue->pq_element_priority = NULL;
    priority_queue->next = NULL;

    priority_queue->copy_element = copy_element;
    priority_queue->free_element = free_element;
    priority_queue->equal_elements = equal_elements;

    priority_queue->copy_priority = copy_priority;
    priority_queue->free_priority = free_priority;
    priority_queue->compare_priorities = compare_priorities;
}

void pqDestroy(PriorityQueue queue)
{
    if(!queue)
    {
        return;
    }
    pqDestroy(queue->next);
    free(queue);
}

PriorityQueue pqCopy(PriorityQueue queue)
{
    if(!pqCopy)
    {
        return NULL;
    }
    
    PriorityQueue newPQ = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements,
                            queue->copy_priority, queue->free_priority, queue->compare_priorities);


    if(!newPQ)
    {
        return NULL;
    }

    /*
        TODO!!!
    */
}

int pqGetSize(PriorityQueue queue)
{
    if(!queue)
    {
        return NULL_OBJECT;
    }

    return queue->elemnts_counter;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    if(!queue || !element)
    {
        return false;
    }

    for(PriorityQueue ptr = queue; ptr != NULL; ptr = ptr->next)
    {
        if(queue->equal_elements(ptr->pq_element, element))
        {
            return true;
        }
    }

    return false;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if(!queue || !element || !priority)
    {
        return PQ_NULL_ARGUMENT;
    } 

    PQElement copyElement = queue->copy_element(element);
    PQElementPriority copyPriority = queue->copy_priority(priority);
    
    if(!copyElement)
    {
        return PQ_OUT_OF_MEMORY;  
    }
    
    if(!copyPriority)
    {
        return PQ_OUT_OF_MEMORY;
    }
    
    //nash nnsa yash sekoy l copy
    PriorityQueue newPQ;

    
    if(queue->compare_priorities(priority, queue->pq_element_priority))
    {
        newPQ->next = queue;
        queue = pqCopy(newPQ);
    }

    /*
    for(PriorityQueue ptr = queue; ptr != NULL; ptr = ptr->next)
    {
        if(queue->compare_priorities(priority, queue->pq_element_priority))
        {

        }
    }
    */
}


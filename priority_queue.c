#include "priority_queue.h"
#include <assert.h>
#include <stdlib.h>

#define NULL_OBJECT -1
#define ZERO 0

typedef struct node_t {
    PQElement elementNode;
    PQElementPriority elementPriorityNode;
    struct node_t* next;
} *Node;

struct PriorityQueue_t {
    Node node;
    int elements_counter;
    int iterator; //*******

    /* Elemetn operations */ 
    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;

    /* priority operations */
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priorities;
};

static Node createNode()
{
    Node node = malloc(sizeof(*node));
    if(!node)
    {
        return NULL;
    }

    node->elementNode = NULL;
    node->elementPriorityNode = NULL;
    node->next = NULL;

    return node;
}

// if node is null, return
static void nodesDestroy(PriorityQueue queue)
{
    Node nodeDelete = queue->node;
    
    while (nodeDelete)
    {
        Node toDelete = nodeDelete;
        queue->free_element(toDelete->elementNode);
        queue->free_priority(toDelete->elementPriorityNode);
        nodeDelete = nodeDelete->next;
        free(toDelete);
    }
}

PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities)
{
    if (!copy_element || !free_element || !equal_elements || !copy_priority || !free_priority || !compare_priorities)
    {
        return NULL;
    }

    PriorityQueue priority_queue = malloc(sizeof(*priority_queue));
    
    if (priority_queue == NULL)
    {
        return NULL;
    }

    Node node = createNode();
    
    priority_queue->node = node;
    priority_queue->elements_counter = 0;
    priority_queue->iterator = 0;

    priority_queue->copy_element = copy_element;
    priority_queue->free_element = free_element;
    priority_queue->equal_elements = equal_elements;

    priority_queue->copy_priority = copy_priority;
    priority_queue->free_priority = free_priority;
    priority_queue->compare_priorities = compare_priorities;
    
    return priority_queue;
}

void pqDestroy(PriorityQueue queue)
{
     if (!queue)
    {
        return;
    }

    nodesDestroy(queue);
    free(queue);
}


PriorityQueue pqCopy(PriorityQueue queue)
{
    if (!queue)
    {
        return NULL;
    }
    
    PriorityQueue newPQ = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements,
                            queue->copy_priority, queue->free_priority, queue->compare_priorities);


    if (!newPQ)
    {
        return NULL;
    }

    newPQ->elements_counter = queue->elements_counter;
    newPQ->iterator = queue->iterator;
    
    Node node = queue->node;

    while (node)
    {
        newPQ->node->elementNode = queue->copy_element(node->elementNode);
        newPQ->node->elementPriorityNode = queue->copy_priority(node->elementPriorityNode);
        node = node->next;
    }

    return newPQ;
}

int pqGetSize(PriorityQueue queue)
{
    if (!queue)
    {
        return NULL_OBJECT;
    }

    return queue->elements_counter; 
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    if (!queue || !element)
    {
        return false;
    }

    for (Node ptr = queue->node; ptr != NULL; ptr = ptr->next)
    {
        if (queue->equal_elements(ptr->elementNode, element))
        {
            return true;
        }
    }

    return false;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if (!queue || !element || !priority)
    {
        return PQ_NULL_ARGUMENT;
    } 

    PQElement copyElement = queue->copy_element(element);
    PQElementPriority copyPriority = queue->copy_priority(priority);
    
    if (!copyElement)
    {
        return PQ_OUT_OF_MEMORY;  
    }
    
    if (!copyPriority)
    {
        return PQ_OUT_OF_MEMORY;
    }

    Node head = queue->node;
    Node nodeAdd = createNode();

    nodeAdd->elementNode = copyElement;
    nodeAdd->elementPriorityNode = copyPriority;

    if (queue->compare_priorities(priority, head->elementPriorityNode) > ZERO || queue->node == NULL)
    {
        nodeAdd->next = head;
        queue->elements_counter++;
        // link the head to queue
    }
    
    else
    {
        while (queue->compare_priorities(priority, head->elementPriorityNode) <= ZERO && head->next != NULL)
        {
            head = head->next;
        }
        if (head->next)
        {   
            if(head->next->next)
            {
                Node temp = head->next->next;
                head->next = nodeAdd; 
                nodeAdd->next = temp;
                queue->elements_counter++;
            }
            else
            {
                head->next = nodeAdd;
                queue->elements_counter++;
                nodeAdd->next = NULL;
            }
        }
        else
        {
            head->next = nodeAdd;
            queue->elements_counter++;
            nodeAdd->next = NULL;
        }
    }

    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                    PQElementPriority old_priority, PQElementPriority new_priority)
{
    if (!queue || !element || !old_priority || !new_priority)
    {
        return 	PQ_NULL_ARGUMENT;
    }

    PQElement copyElement = queue->copy_element(element);
    PQElementPriority copyPriority = queue->copy_priority(new_priority);

    if (!copyElement)
    {
        return PQ_OUT_OF_MEMORY;
    }

    if (!copyPriority)
    {
        return PQ_OUT_OF_MEMORY;
    }

    if ( !(pqContains(queue, element)) )
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }

    Node currentNode = queue->node;
    Node tempNode = currentNode;

    while ( !(queue->equal_elements(currentNode->elementNode, element)) 
        || !(queue->compare_priorities(currentNode->elementPriorityNode, old_priority)) )
    {
        tempNode = currentNode;
        currentNode = currentNode->next;

        if (currentNode->next == NULL)
        {
            return PQ_ELEMENT_DOES_NOT_EXISTS;
        }
    }

    tempNode->next = currentNode->next;

    queue->free_element(currentNode->elementNode);
    queue->free_priority(currentNode->elementPriorityNode);
    free(currentNode);

    queue->elements_counter--;

    pqInsert(queue, element, new_priority);

    return PQ_SUCCESS;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    if (!queue)
    {
        return PQ_NULL_ARGUMENT;
    }

    if (queue->node == NULL)
    {
        return PQ_SUCCESS;
    }

    Node toDelete = queue->node;

    queue->free_element(toDelete->elementNode);
    queue->free_priority(toDelete->elementPriorityNode);
    
    queue->node = toDelete->next;
    queue->elements_counter--;

    free(toDelete);

    return PQ_SUCCESS; 
}


PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    if(!queue || !element)
    {
        return PQ_NULL_ARGUMENT;
    }

    if ( !(pqContains(queue, element)) )
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }

    else
    {
        if (queue->equal_elements(queue->node->elementNode, element))
        {
            pqRemove(queue);
            return PQ_SUCCESS;
        }
        
        Node currentNode = queue->node;
        Node previousCurrentNode = currentNode;

        while ( !(queue->equal_elements(element, currentNode->elementNode)) )
        {
            previousCurrentNode = currentNode;
            currentNode = currentNode->next;
        }

        if (currentNode->next)
        {
            Node nextCurrentNodeTemp = currentNode->next;
            
            queue->free_element(currentNode->elementNode);
            queue->free_priority(currentNode->elementPriorityNode);
            free(currentNode);

            queue->elements_counter--;

            previousCurrentNode->next = nextCurrentNodeTemp;
        }
        else
        {
            queue->free_element(currentNode->elementNode);
            queue->free_priority(currentNode->elementPriorityNode);
            free(currentNode);
            queue->elements_counter--;
            previousCurrentNode->next = NULL;
        }
    }

    return PQ_SUCCESS;
}

PQElement pqGetFirst(PriorityQueue queue)
{
    if (!queue || queue->elements_counter == 0)
    {
        return NULL;
    }

    queue->iterator = 0;
    return pqGetNext(queue);
}

PQElement pqGetNext(PriorityQueue queue)
{
    if (!queue || queue->elements_counter <= queue->iterator)
    {
        return NULL;
    }
    queue->iterator++;
    return queue->node->elementNode;
}

PriorityQueueResult pqClear(PriorityQueue queue)
{
    if (!queue)
    {
        return PQ_NULL_ARGUMENT; 
    }

    Node currentNode = queue->node;
    Node toDelete = currentNode;

    queue->elements_counter = 0;

    while(currentNode)
    {
        toDelete = currentNode;
        queue->free_element(currentNode->elementNode);
        queue->free_priority(currentNode->elementPriorityNode);
        currentNode = currentNode->next;
        
        free(toDelete);
    }
    
    return PQ_SUCCESS;
}








/*
struct PriorityQueue_t {
    PQElement pq_element;
    PQElementPriority pq_element_priority;
    int elemnts_counter;
    struct PriorityQueue_t* next;

    /* Elemetn operations */ 
    /*
    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;

    /* priority operations */
    /*
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
    if (!copy_element || !free_element || !equal_elements || !copy_priority || !free_priority || !compare_priorities)
    {
        return NULL;
    }

    PriorityQueue priority_queue = malloc(sizeof(*priority_queue));
    if (priority_queue == NULL)
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

    return priority_queue;
}

void pqDestroy(PriorityQueue queue)
{
    if (!queue)
    {
        return;
    }

    pqDestroy(queue->next);
    queue->free_element(queue->pq_element);
    queue->free_priority(queue->pq_element_priority);
    free(queue);

    /*
    if (queue->next)
    {
        PriorityQueue currentNode = queue->next;

        while (currentNode)
        {
            queue->free_element(currentNode->pq_element);
            queue->free_priority(currentNode->pq_element_priority);
            currentNode = currentNode->next;
        }

        currentNode = queue->next;
        
        while (currentNode)
        {
            PriorityQueue toDelete = currentNode;
            currentNode = currentNode->next;
            free (toDelete);
        }
    }
    free (queue);
    
}

PriorityQueue pqCopy(PriorityQueue queue)
{
    if (!pqCopy)
    {
        return NULL;
    }
    
    PriorityQueue newPQ = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements,
                            queue->copy_priority, queue->free_priority, queue->compare_priorities);


    if (!newPQ)
    {
        return NULL;
    }
    
    for (PriorityQueue ptr = queue; ptr != NULL; ptr = ptr->next)
    {
        newPQ->pq_element = ptr->pq_element;
        newPQ->pq_element_priority = ptr->pq_element_priority;
        newPQ->next = ptr->next;
    }

    return newPQ;
}

int pqGetSize(PriorityQueue queue)
{
    if (!queue)
    {
        return NULL_OBJECT;
    }

    return queue->elemnts_counter;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    if (!queue || !element)
    {
        return false;
    }

    for (PriorityQueue ptr = queue; ptr != NULL; ptr = ptr->next)
    {
        if (queue->equal_elements(ptr->pq_element, element))
        {
            return true;
        }
    }

    return false;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if (!queue || !element || !priority)
    {
        return PQ_NULL_ARGUMENT;
    } 

    PQElement copyElement = queue->copy_element(element);
    PQElementPriority copyPriority = queue->copy_priority(priority);
    
    if (!copyElement)
    {
        return PQ_OUT_OF_MEMORY;  
    }
    
    if (!copyPriority)
    {
        return PQ_OUT_OF_MEMORY;
    }

    PriorityQueue head;


}
*/


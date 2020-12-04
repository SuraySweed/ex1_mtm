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
    int currentNodeNumber;
    PQElement iterator; 

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
    if (!node)
    {
        return NULL;
    }

    node->elementNode = NULL;
    node->elementPriorityNode = NULL;
    node->next = NULL;

    return node;
}

static Node copyNodes(PriorityQueue queue, Node node)
{
    if (node == NULL)
    {
        return NULL;
    }

    Node copyNode = createNode();
    copyNode->elementNode = queue->copy_element(node->elementNode);
    copyNode->elementPriorityNode = queue->copy_priority(node->elementPriorityNode);
    copyNode->next = copyNodes(queue, node->next);

    return copyNode;
}

// if node is null, return
static void nodesDestroy(PriorityQueue queue)
{
    Node nodeDelete = queue->node;

    while (nodeDelete)
    {
        queue->free_element(nodeDelete->elementNode);
        queue->free_priority(nodeDelete->elementPriorityNode);
        nodeDelete = nodeDelete->next;
    }

    nodeDelete = queue->node;
    
    while (nodeDelete)
    {
        Node toDelete = nodeDelete;
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

    priority_queue->node = NULL;
    priority_queue->elements_counter = 0;
    priority_queue->currentNodeNumber = 0;
    priority_queue->iterator = NULL;

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

    queue->iterator = NULL;

    PriorityQueue newPQ = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements,
        queue->copy_priority, queue->free_priority, queue->compare_priorities);


    if (!newPQ)
    {
        return NULL;
    }

    newPQ->elements_counter = queue->elements_counter;
    newPQ->iterator = queue->iterator;
    newPQ->currentNodeNumber = 0;
    Node currentNode = queue->node;

    Node copyNode = copyNodes(queue, currentNode);

    newPQ->node = copyNode;

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

    queue->iterator = NULL;

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
    nodeAdd->next = NULL;

    if (!(queue->node))
    {
        queue->node = nodeAdd;
        queue->elements_counter++;
        return PQ_SUCCESS;
    }

    else if (queue->compare_priorities(priority, head->elementPriorityNode) > ZERO)
    {
        nodeAdd->next = queue->node;
        queue->node = nodeAdd;

        queue->elements_counter++;
        return PQ_SUCCESS;
    }

    else
    {

        while (queue->compare_priorities(priority, head->elementPriorityNode) < ZERO && head->next != NULL &&
            queue->compare_priorities(priority, head->next->elementPriorityNode) < ZERO)
        {
            head = head->next;
        }
        
        while ( (queue->compare_priorities(priority, head->elementPriorityNode) == ZERO) && head->next != NULL && 
            (queue->compare_priorities(priority, head->next->elementPriorityNode) == ZERO))
        {
            head = head->next;
        }

        if (head->next)
        {
            Node temp = head->next;
            head->next = nodeAdd;
            nodeAdd->next = temp;

            queue->elements_counter++;
            return PQ_SUCCESS;
        }
        else
        {
            head->next = nodeAdd;
            queue->elements_counter++;
            return PQ_SUCCESS;
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

    queue->iterator = NULL;
    PQElement copyElement = queue->copy_element(element);
    PQElementPriority copyPriority = queue->copy_priority(new_priority);

    if (!copyElement)
    {
        queue->free_element(copyElement);
        queue->free_priority(copyPriority);

        return PQ_OUT_OF_MEMORY;
    }

    if (!copyPriority)
    {
        queue->free_element(copyElement);
        queue->free_priority(copyPriority);

        return PQ_OUT_OF_MEMORY;
    }

    if (!(pqContains(queue, element)))
    {
        queue->free_element(copyElement);
        queue->free_priority(copyPriority);

        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }

    Node currentNode = queue->node;
    Node tempNode = queue->node;

 
    while ((queue->compare_priorities(currentNode->elementPriorityNode, old_priority)) != 0
        && !(queue->equal_elements(currentNode->elementNode, element)))
    {
        // if we exit the loop, currentNode is the node that we have to change it
        tempNode = currentNode;
        currentNode = currentNode->next;

        if (currentNode == NULL || currentNode->next == NULL)
        {
            queue->free_element(copyElement);
            queue->free_priority(copyPriority);

            return PQ_ELEMENT_DOES_NOT_EXISTS;
        }
    }

    tempNode->next = currentNode->next;

    queue->free_element(currentNode->elementNode);
    queue->free_priority(currentNode->elementPriorityNode);

    free(currentNode);
    
    if (queue->elements_counter == 1)
    {
        queue->node = NULL;
    }

    queue->elements_counter--;

    pqInsert(queue, copyElement, copyPriority);

    queue->free_element(copyElement);
    queue->free_priority(copyPriority);

    return PQ_SUCCESS;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    if (!queue)
    {
        return PQ_NULL_ARGUMENT;
    }
    
    queue->iterator = NULL;

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
    if (!queue || !element)
    {
        return PQ_NULL_ARGUMENT;
    }

    queue->iterator = NULL;

    if (!(pqContains(queue, element)))
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

        while (!(queue->equal_elements(element, currentNode->elementNode)))
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
    queue->currentNodeNumber = 1;
    queue->iterator = queue->node->elementNode;
    return queue->iterator;
}

PQElement pqGetNext(PriorityQueue queue)
{
    if (!queue || queue->iterator == NULL)
    {
        return NULL;
    }


    int counter = 1;
    Node currentNode;

    for (currentNode = queue->node; counter < queue->currentNodeNumber; counter++, currentNode = currentNode->next);
    queue->currentNodeNumber++;

    if (currentNode->next == NULL)
    {
        return NULL;
    }


    queue->iterator = currentNode->next->elementNode;

    return queue->iterator;
}

PriorityQueueResult pqClear(PriorityQueue queue)
{
    if (!queue)
    {
        return PQ_NULL_ARGUMENT;
    }

    Node currentNode = queue->node;

    queue->elements_counter = 0;

    while (currentNode)
    {
        Node toDelete = currentNode; 
        queue->free_element(currentNode->elementNode);
        queue->free_priority(currentNode->elementPriorityNode);
        currentNode = currentNode->next;

        free(toDelete);
    }
    
    queue->node = NULL;
    queue->elements_counter = 0;
    queue->currentNodeNumber = 0;
    queue->iterator = NULL;

    return PQ_SUCCESS;
}


#include "priority_queue.h"


struct PriorityQueue_t {
    PQElement element;
    PQElementPriority priority;
    int size;
    struct PriorityQueue_t next;
};
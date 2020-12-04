#ifndef EVENTS_H
#define EVENTS_H

#include "date.h"
#include "priority_queue.h"

typedef PriorityQueue Events;

typedef struct EventElement_t *EventElement; //PQElement
typedef Date EventDate; // PQElementPriority


#endif //EVENTS_H

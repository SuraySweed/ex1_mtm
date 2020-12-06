#ifndef EVENTS_H
#define EVENTS_H

#include "date.h"
#include "priority_queue.h"
#include "event_members.h"

typedef PriorityQueue Events;

typedef struct EventsElement_t* EventsElement;
Events createEvents();

bool changeDateInElementInEvents(Events events, Date date, int id);

//PQElement createEventElement(char *name, int id, Date date);
void destroyEvents(Events events);

#endif //EVENTS_H

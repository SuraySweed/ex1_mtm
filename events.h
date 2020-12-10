#ifndef EVENTS_H
#define EVENTS_H

#include "date.h"
#include "priority_queue.h"
#include "event_members.h"

typedef PriorityQueue Events;

typedef struct EventsElement_t* EventsElement;
Events createEvents();

char* getEventName(EventsElement event_element);
Date getEventDate(EventsElement event_element);
EventsElement getEventByEventId(Events events, int id);
bool changeDateInElementInEvents(Events events, Date date, int id);

EventsElement createEventsElement(char* name,int id, Date date, EventMembers event_members);

//PQElement createEventElement(char *name, int id, Date date);
void destroyEvents(Events events);

#endif //EVENTS_H

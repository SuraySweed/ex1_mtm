#ifndef EVENTS_H
#define EVENTS_H

#include "date.h"
#include "priority_queue.h"
#include "event_members.h"
#include "members.h"

typedef PriorityQueue Events;

typedef struct EventsElement_t* EventsElement;
Events createEvents();

char* getEventName(EventsElement event_element);
Date getEventDate(EventsElement event_element);
EventsElement getEventByEventId(Events events, int id);
EventMembers getEventMembersByEvent(EventsElement event);
bool changeDateInElementInEvents(Events events, Date date, int id);

void changeMemberCounterPriorityWhenRemoveEvent(Members members, int member_id);
bool removeOccursEvents(Members members, Events events, Date current_date);

EventsElement createEventsElement(char* name, int id, Date date, EventMembers event_members);

void destroyEventElement(EventsElement event_element);
void destroyEvents(Events events);

#endif //EVENTS_H

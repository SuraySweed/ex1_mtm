#ifndef EVENT_MEMBERS_H
#define EVENT_MEMBERS_H

#include "priority_queue.h"

typedef PriorityQueue EventMembers;

EventMembers createEventMembers();

void destroyEventMembers(EventMembers event_members);


#endif //EVENT_MEMBERS_H


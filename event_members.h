#ifndef EVENT_MEMBERS_H
#define EVENT_MEMBERS_H

#include <stdbool.h>
#include "priority_queue.h"

typedef PriorityQueue EventMembers;
typedef struct EventMemberElement_t* EventMemberElement;

EventMembers createEventMembers();
EventMemberElement createEventMemberElement(char* name, int id);

int getIdByEventMemberElement(EventMemberElement event_member_element);
char* getNameByEventMemberElement(EventMemberElement event_members_element);

void destroyEventMemberElement(EventMemberElement event_members_element);
void destroyEventMembers(EventMembers event_members);


#endif //EVENT_MEMBERS_H


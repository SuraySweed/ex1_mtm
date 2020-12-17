#ifndef EVENT_MEMBERS_H
#define EVENT_MEMBERS_H

#include <stdbool.h>
#include "priority_queue.h"


/**
* event members for all the member that participate in a 
* specific event. that implemeted by Priority Queue Container
*
* Implements an events container type.
* The following functions are available:
*   createEventMembers					- Creates a new empty priority queue
*   destroyEventMembers					- Deletes an existing priority queue and frees all resources
*   getNameByEventMemberElement			- Returns the name of a given element
*	getIdByEventMemberElement			- Returns the id of a given element
*   getEventByEventId					- Returns the event of a given events and id
*	createEventMemberElement			- Create a new element
*	destroyEventMembers					- Deletes an existing element and frees all resources
*/



/*Type for defining the event members by priority queue*/
typedef PriorityQueue EventMembers;

/*Data element for events*/
typedef struct EventMemberElement_t* EventMemberElement;

/*
	createEventMembers: allocate a new empty event members(priority queue) by pqCreate
	with a 6 static functions to the pq(free, compare and copy) for the element and the priority
	return the returned value by pqCreate
*/
EventMembers createEventMembers();

/*
	createEventMembersElement: create a new element
	@param name - the name of the member
	@param id - the id of the members
	@return:
		NULL if allocations failed,
		otherwise return a new event members
*/
EventMemberElement createEventMemberElement(char* name, int id);

/*
	getIdByEventMemberElement: return the id of specific member
	@param event_members_element - the element of the pq
	@return
		NULL if a NULL pointer was sent
		othwerwise the id of the element
*/
int getIdByEventMemberElement(EventMemberElement event_member_element);

/*
	getNameByEventMemberElement: return the name of specific member
	@param event_members_element - the element of the pq
	@return
		NULL if a NULL pointer was sent
		othwerwise the name of the element
*/
char* getNameByEventMemberElement(EventMemberElement event_members_element);

/*
	destroyEventMemberElement: deallocates an existing element.
	@param event_members_element - the element that we want to free
*/
void destroyEventMemberElement(EventMemberElement event_members_element);

/*
	destroyEvents: Deallocates an existing priority queue. Clears all elements by using the free functions.
	@param events - target pq to be deallocated. if pq is NULL nothing will be done
*/
void destroyEventMembers(EventMembers event_members);


#endif //EVENT_MEMBERS_H


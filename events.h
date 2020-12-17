#ifndef EVENTS_H
#define EVENTS_H

#include "date.h"
#include "priority_queue.h"
#include "event_members.h"
#include "members.h"


/**
* events that implemeted by Priority Queue Container
*
* Implements an events container type.
* The following functions are available:
*   createEvents								- Creates a new empty priority queue
*   destroyEvents								- Deletes an existing priority queue and frees all resources
*   getEventName								- Returns the name of a given event(element)
*	getEventDate								- Returns the date of a given event(element)
*   getEventByEventId							- Returns the event of a given events and id
*	getEventMembersByEvent						- Returns the event members of a given pq
*   changeDateInElementInEvents					- Change the date in the element
*	changeMemberCounterPriorityWhenRemoveEvent  - Chane the event counter in member
*	removeOccursEvents							- Remove occurs events in pq events
*	createEventsElement							- Create a new element
*	destroyEventElement							- Deletes an existing element and frees all resources
*/


/*Type for defining the events by priority queue*/
typedef PriorityQueue Events;

/*Data element for events*/
typedef struct EventsElement_t* EventsElement;

/*
	createEvents: allocate a new empty events(priority queue) by pqCreate
	with a 6 static functions to the pq(free, compare and copy) for the element and the priority
	return the returned value by pqCreate
*/
Events createEvents();

/*
	getEventName: return the name of specific event
	@param event_element - the element of the pq
	@return
		NULL if a NULL pointer was sent
		othwerwise the name of the element
*/
char* getEventName(EventsElement event_element);

/*
	getEventDate: return the date of specific event
	@param event_element - the element of the pq
	@return
		NULL if a NULL pointer was sent
		othwerwise the date of the element
*/
Date getEventDate(EventsElement event_element);

/*
	getEventByEventId: return specific event
	@param events - priority queue (Events) to search in
	@param id - the event id that we have to return
	@return
		NULL if a NULL pointer was sent
		othwerwise return the event 
*/
EventsElement getEventByEventId(Events events, int id);

/*
	getEventMembersByEvent: return the pq eventMembers
	@param event - specific events element- element.
	@return
		NULL f a NULL pointer was sent
		otherwise return the eventMembers pq
*/
EventMembers getEventMembersByEvent(EventsElement event);

/*
	changeDateInElementInEvents: change the date in the specific element that have id
	@param events - pq to search in
	@param date - the new date to set in the specific event
	@param id - the event id that we have to change it
	@return
		false if the event_id not found in the events, 
		true if we found the event and change his date
*/
bool changeDateInElementInEvents(Events events, Date date, int id);

/*
	changeMemberCounterPriorityWhenRemoveEvent: decrease 1 from the event_counter for the members who 
	participate the event that we want to remove
	@param members - pq to search in
	@param member_id - the id of the member that we want to change his priority
*/
void changeMemberCounterPriorityWhenRemoveEvent(Members members, int member_id);

/*
	rempveOccursEvents: remove all the events that has occues(before current_date)
	@param members - we want this pq because we have to change the event_counter to the member that particapte in
	all the events that occurs
	@param events - pq to search in 
	@param current_date - that current date in the em.
	@return:
		false if NULL was sent, or if allocations failed
		true if we have succses in removing the events and change the priority
*/
bool removeOccursEvents(Members members, Events events, Date current_date);

/*
	createEventsElement: create a new element
	@param name - the name of the event
	@param id - the id of the event
	@param date - the date of the event
	@param event_members - the members that participate in the event
	@return:
		NULL if allocations failed,
		otherwise return a new event_element
*/
EventsElement createEventsElement(char* name, int id, Date date, EventMembers event_members);

/*
	destroyEventElement: deallocates an existing element.
	@param event_element - the element that we want to free
*/
void destroyEventElement(EventsElement event_element);

/*
	destroyEvents: Deallocates an existing priority queue. Clears all elements by using the free functions.
	@param events - target pq to be deallocated. if pq is NULL nothing will be done
*/
void destroyEvents(Events events);

#endif //EVENTS_H

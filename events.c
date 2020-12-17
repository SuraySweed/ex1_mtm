#include "events.h"
#include "priority_queue.h"
#include "date.h"
#include "event_members.h"
#include "members.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ZERO 0

struct EventsElement_t {
    char* name;
    int id;
    Date date;
    EventMembers event_members;

};

/*type of function for deallocationg a data element of the events*/
static void freeElementStructGeneric(PQElement element_struct)
{
    free(((EventsElement)element_struct)->name);
    dateDestroy((Date)((EventsElement)element_struct)->date);
    destroyEventMembers(((EventsElement)element_struct)->event_members);
    free(element_struct); 
}

/*Type of function for deallocating a key element of the events*/
static void freePriorityElementDateGeneric(PQElementPriority date)
{
    dateDestroy(((Date)date));
}

/*Type of function for copying a data element of the priority queue */
static PQElement copyStructElementGeneric(PQElement element_struct)
{
    if (!element_struct)
    {
        return NULL;
    }


    EventsElement copy_element = malloc((sizeof(*copy_element)));

    if (!copy_element)
    {
        return NULL;
    }

    int length = 0;

    length = strlen(((EventsElement)element_struct)->name);
    copy_element->name = malloc((sizeof(char*) * length) + 1);

    if (!copy_element->name)
    {
        freeElementStructGeneric(copy_element);
        return NULL;
    }

    strcpy(copy_element->name, ((EventsElement)element_struct)->name);
    copy_element->id = ((EventsElement)element_struct)->id;
    copy_element->date = dateCopy(((EventsElement)element_struct)->date);
    if (!copy_element->date)
    {
        freeElementStructGeneric(copy_element);
        return NULL;
    }
    
    copy_element->event_members = pqCopy(((EventsElement)element_struct)->event_members);
    if (!copy_element->event_members)
    {
        freeElementStructGeneric(copy_element);
        return NULL;
    }

    return copy_element;
}

/** Type of function for copying a data key element of the priority queue */
static PQElementPriority copyPriorityElementDateGeneric(PQElementPriority date)
{
    if (!date)
    {
        return NULL;
    }

    Date copy_date = dateCopy((Date)date);

    return copy_date;
}

/**
* Type of function used by the events to compare priorities(dates).
* This function should return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
*		A negative integer if the second element is greater.
*/
static int compareDatePriorityElementGeneric(PQElementPriority date, PQElementPriority current_date_in_em)
{
    return dateCompare(current_date_in_em, date);
}

/**
* Type of function used by the priority queue to identify equal elements.
* This function should return:
* 		true if they're equal;
*		false otherwise;
*/
static bool equalIdElementGeneric(PQElement element1, PQElement element2)
{
    int is_equal_id = 0;

    is_equal_id = (((EventsElement)element1)->id) - (((EventsElement)element2)->id);

    return is_equal_id == 0 ? true : false;
}

// create a new events
Events createEvents()
{
    Events events = pqCreate(copyStructElementGeneric, freeElementStructGeneric, equalIdElementGeneric,
        copyPriorityElementDateGeneric, freePriorityElementDateGeneric, compareDatePriorityElementGeneric);

    return events;
}

// return the name of the event_element
char* getEventName(EventsElement event_element)
{
    if (!event_element)
    {
        return NULL;
    }

    return event_element->name;
}

// return the date of the event_element
Date getEventDate(EventsElement event_element)
{
    if (!event_element)
    {
        return NULL;
    }

    return event_element->date;
}

// return event by event_id
EventsElement getEventByEventId(Events events, int id)
{
    if (pqGetSize(events) == 0)
    {
        return NULL;
    }

    EventsElement event_element = pqGetFirst(events);
    if (!event_element)
    {
        return NULL;
    }

    if (event_element->id == id)
    {
        return event_element;
    }

    while (event_element && event_element->id != id)
    {
        event_element = pqGetNext(events);
        
        if (event_element && event_element->id == id)
        {
            return event_element;
        }
    }

    return NULL;
}

// return event_members in by the sent event
EventMembers getEventMembersByEvent(EventsElement event)
{
    if (!event)
    {
        return NULL;
    }

    return event->event_members;
}

// change the date in a specific element 
bool changeDateInElementInEvents(Events events, Date date, int id)
{
    if (!events || !date)
    {
        return false;
    }

    EventsElement found_element;
    found_element = ((EventsElement)pqGetFirst(events));

    if (!found_element)
    {
        return false;
    }

    if (found_element->id == id)
    {
        dateDestroy(found_element->date);
        found_element->date = dateCopy(date);
        if (!found_element->date)
        {
            return false;
        }
        return true;
    }

    while (found_element->id != id)
    {
        found_element = ((EventsElement)pqGetNext(events));

        if (found_element->id == id)
        {
            dateDestroy(found_element->date);
            found_element->date = dateCopy(date);
            if (!found_element->date)
            {
                return false;
            }
            return true;
        }
    }

    return false;
}

// change the priority of the members if we remove some event
void changeMemberCounterPriorityWhenRemoveEvent(Members members, int member_id)
{
    MembersElement member_element =
        createMemberElement(getMemberNameByMember(getMemberById(members, member_id)), member_id,
            getEventsCounterInMember(getMemberById(members, member_id)));

    MembersPriority member_priority =
        createMemberPriority(getEventsCounterInMember(getMemberById(members, member_id)),
            getMemberIdByMember(getMemberById(members, member_id)));


    MembersPriority new_member_priority =
        createMemberPriority((getEventsCounterInMember(getMemberById(members, member_id)) - 1),
            getMemberIdByMember(getMemberById(members, member_id)));


    PriorityQueueResult change_priority_result = pqChangePriority(members, member_element, member_priority,
        new_member_priority);

    if (change_priority_result == PQ_OUT_OF_MEMORY)
    {

        destroyMembersElement(member_element);
        destroyMemberPriority(member_priority);
        destroyMemberPriority(new_member_priority);
        return;
    }

    destroyMembersElement(member_element);
    destroyMemberPriority(member_priority);
    destroyMemberPriority(new_member_priority);
}

// remove all the occures events according to the current_Date
bool removeOccursEvents(Members members, Events events, Date current_date)
{
    if (!members || !events || !current_date)
    {
        return false;
    }

    EventsElement current_event = pqGetFirst(events);

    if (!current_event)
    {
        return false;
    }


    EventMemberElement event_member_element;

    int member_id = 0;

    while (current_event && dateCompare(current_event->date, current_date) < ZERO)
    {
        event_member_element = pqGetFirst(getEventMembersByEvent(current_event));

        while (event_member_element)
        {
            
            member_id = getIdByEventMemberElement(event_member_element);

            changeMemberCounterPriorityWhenRemoveEvent(members, member_id);
            subEventsCounterInMember(getMemberById(members, member_id));

            event_member_element = pqGetNext(getEventMembersByEvent(current_event));

        }
        pqRemove(events);
        current_event = pqGetFirst(events);
    }

    return true;
}

// reutrn a new event element 
EventsElement createEventsElement(char* name, int id, Date date, EventMembers event_members)
{
    if (!date || !name)
    {
        return NULL;
    }

    EventsElement event_element = malloc((sizeof(*event_element)));

    if (!event_element)
    {
        return NULL;
    }

    int name_length = strlen(name);
    event_element->name = malloc((sizeof(char) * name_length) + 1);

    if (!(event_element->name))
    {
        freeElementStructGeneric(event_element);
        return NULL;
    }

    strncpy(event_element->name, name, name_length + 1);
    event_element->id = id;
    event_element->date = dateCopy(date);
    if (!event_element->date)
    {
        freeElementStructGeneric(event_element);
        return NULL;
    }
    
    event_element->event_members = createEventMembers();
    return event_element;

}

// destroy event element
void destroyEventElement(EventsElement event_element)
{
    if (!event_element)
    {
        return;
    }

    freeElementStructGeneric(event_element);
}

// destroy events
void destroyEvents(Events events)
{
    pqDestroy(events);
}

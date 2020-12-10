#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"
#include "events.h"
#include "members.h"
#include "event_members.h"

#define ZERO 0

struct EventManager_t {
    Events events;      // priority queue
    Members members;    // priority queue
    Date current_date;
};

static bool isValidId(int id)
{
    if (id > 0)
    {
        return true;
    }

    return false;
}

/*
    check if date is not grater than current_date
*/
static bool isValidDateAccrdingToCurrentDate(Date date, Date current_date)
{
    if (dateCompare(date, current_date) >= ZERO) // date arrives after current_date
    {
        return true;
    }

    return false;
}

static bool EventsContainEventNameInDate(EventManager em, Date date, char* event_name)
{
    EventsElement current_event = pqGetFirst(em->events);

    if (!current_event)
    {
        return NULL;
    }

    while (current_event)
    {
        if (dateCompare(getEventDate(current_event), date) == 0)
        {
            if (strcmp(getEventName(current_event), event_name) == 0)
            {
                return true;
            }
        }

        current_event = pqGetNext(em->events);
    }

    return false;
}

EventManager createEventManager(Date date)
{
    if (!date)
    {
        return NULL;
    }

    Date copy_date = dateCopy(date);
    EventManager event_manager = malloc(sizeof(*event_manager));

    if (!event_manager)
    {
        return NULL;
    }

    event_manager->current_date = copy_date;
    event_manager->events = NULL;
    event_manager->members = NULL;

    return event_manager;
}

void destroyEventManager(EventManager em)
{
    if (!em)
    {
        return;
    }

    dateDestroy(em->current_date);
    destroyMembers(em->members);
    destroyEvents(em->events);

    free(em);
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    if (!em || !event_name || !date)
    {
        return EM_NULL_ARGUMENT;
    }

    if (!isValidDateAccrdingToCurrentDate(date, em->current_date))
    {
        return EM_INVALID_DATE;
    }

    if (!isValidId(event_id))
    {
        return EM_INVALID_EVENT_ID;
    }

    if (EventsContainEventNameInDate(em, date, event_name))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }

    if (pqContains(em->events, getEventByEventId(em->events, event_id)))
    {
        return EM_EVENT_ID_ALREADY_EXISTS;
    }

    EventsElement event_element = createEventsElement(event_name, event_id, date, em->members);
    
    if (!event_element)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    pqInsert(em->events, event_element, date);

    return EM_SUCCESS;
}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    Date new_date = dateCopy(em->current_date);

    if (!new_date)
    {
        return NULL;
    }

    // add days to the current event, and save the new date in new_date
    for (int current = 0; current < days; current++)
    {
        dateTick(new_date);
    }

    emAddEventByDate(em, event_name, new_date, event_id);
}

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
    if (!em)
    {
        return EM_NULL_ARGUMENT;
    }

    if (!isValidId(event_id))
    {
        return EM_INVALID_EVENT_ID;
    }

    if (!pqContains(em->current_date, getEventByEventId(em->events, event_id)))
    {
        return EM_EVENT_NOT_EXISTS;
    }

    pqRemoveElement(em->events, getEventByEventId(em->events, event_id));
    
    return EM_SUCCESS;
}

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
    if (!em || !new_date)
    {
        return EM_NULL_ARGUMENT;
    }

    if (!isValidDateAccrdingToCurrentDate(new_date, em->current_date))
    {
        return EM_INVALID_DATE;
    }

    if (!isValidId(event_id))
    {
        return EM_INVALID_EVENT_ID;
    }
    
    EventsElement event = getEventByEventId(em->events, event_id);

    if (!event)
    {
        return EM_OUT_OF_MEMORY;
    }

    if (!pqContains(em->current_date, event))
    {
        return EM_EVENT_NOT_EXISTS;
    }

    if (EventsContainEventNameInDate(em, new_date, getEventName(event)))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }


    pqChangePriority(em->events, event, getEventDate(event) , new_date);
    
    return EM_SUCCESS;
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
    if (!em || !member_name)
    {
        return EM_NULL_ARGUMENT;
    }

    if (!isValidId(member_id))
    {
        return EM_INVALID_MEMBER_ID;
    }

    if (getMemberById(em->members, member_id))
    {
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }

    MembersElement member_element = createMemberElement(member_name, member_id, 0);

    if (!member_element)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    pqInsert(em->members, member_element, 0);

    return EM_SUCCESS;
}

EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
    if (!em)
    {
        return EM_NULL_ARGUMENT;
    }

    if (!isValidId(event_id))
    {
        return EM_INVALID_EVENT_ID;
    }

    if (!isValidId(member_id))
    {
        return EM_INVALID_MEMBER_ID;
    }

    if (!getMemberById(em->members, member_id))
    {
        return EM_MEMBER_ID_NOT_EXISTS;
    }

    if (!getEventByEventId(em->events, event_id))
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }

    //TODO
}







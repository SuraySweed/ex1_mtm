#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "event_manager.h"
#include "date.h"
#include "event_members.h"
#include "events.h"
#include "members.h"
#include "priority_queue.h"


#define ZERO 0
#define NEGATIVE -1

struct EventManager_t {
    Events events;      // priority queue
    Members members;    // priority queue
    Date current_date;

    bool is_first_event_returned;
};

static bool isValidId(int id)
{
    if (id >= 0)
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

    if (!current_event && (em->events))
    {
        return false;
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

static void dateTickByDays(Date date, int days)
{
    for (int current = 0; current < days; current++)
    {
        dateTick(date);
    }
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
    event_manager->events = createEvents();
    event_manager->members = createMembers();
    event_manager->is_first_event_returned = false;

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

    //if (pqContains(em->events, getEventByEventId(em->events, event_id)))
    if (getEventByEventId(em->events, event_id))
    {
        return EM_EVENT_ID_ALREADY_EXISTS;
    }

    EventsElement event_element = createEventsElement(event_name, event_id, date, NULL);

    if (!event_element)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    pqInsert(em->events, event_element, date);

    destroyEventElement(event_element);

    return EM_SUCCESS;
}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    if (!em || !event_name)
    {
        return EM_NULL_ARGUMENT;
    }

    Date new_date = dateCopy(em->current_date);

    if (!new_date)
    {
        return EM_NULL_ARGUMENT;
    }

    // add days to the current event, and save the new date in new_date
    if(days >= 0)
    {
        dateTickByDays(new_date, days);
        return emAddEventByDate(em, event_name, new_date, event_id);
    }
    
    return EM_INVALID_DATE;
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

    //if (!pqContains(em->current_date, getEventByEventId(em->events, event_id)))
    if (!getEventByEventId(em->events, event_id))
    {
        return EM_EVENT_NOT_EXISTS;
    }

    EventMembers event_members = getEventMembersByEvent(getEventByEventId(em->events, event_id));
    EventMemberElement event_member_element = pqGetFirst(event_members);
    
    int member_id = 0;

    while (event_member_element)
    {
        member_id = getIdByEventMemberElement(event_member_element);

        changeMemberCounterPriorityWhenRemoveEvent(em->members, member_id);
        subEventsCounterInMember(getMemberById(em->members, member_id));
        
        event_member_element = pqGetNext(event_members);
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
        return EM_EVENT_ID_NOT_EXISTS;
    }
    /*
    if (!pqContains(em->events, event))
    {
        return EM_EVENT_NOT_EXISTS;
    }
    */
    if (EventsContainEventNameInDate(em, new_date, getEventName(event)))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }


    pqChangePriority(em->events, event, getEventDate(event), new_date);
    changeDateInElementInEvents(em->events, new_date, event_id);

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

    MembersPriority member_priority = createMemberPriority(0, member_id);

    if (!member_priority)
    {
        destroyMembersElement(member_element);
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    pqInsert(em->members, member_element, member_priority);

    destroyMembersElement(member_element);
    destroyMemberPriority(member_priority);

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

    if (!getEventByEventId(em->events, event_id))
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }

    if (!getMemberById(em->members, member_id))
    {
        return EM_MEMBER_ID_NOT_EXISTS;
    }

    EventsElement event = getEventByEventId(em->events, event_id);

    if (!event)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    MembersElement member = getMemberById(em->members, member_id);
    
    if (!member)
    {
        destroyEventElement(event);
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    EventMemberElement event_member_element = createEventMemberElement(getMemberNameByMember(member), member_id);

    if (!event_member_element)
    {
        destroyEventElement(event);
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    if (pqContains(getEventMembersByEvent(event), event_member_element))
    {
        destroyEventMemberElement(event_member_element);
        return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
    }

    pqInsert(getEventMembersByEvent(event), event_member_element, &member_id);
    
    int previous_events_counter = getEventsCounterInMember(member);
    
    addEventsCounterInMember(member);

    int new_events_counter = previous_events_counter + 1;

    MembersPriority privious_member_priority = createMemberPriority(previous_events_counter, member_id);
    MembersPriority new_member_priority = createMemberPriority(new_events_counter, member_id);

    pqChangePriority(em->members, member, privious_member_priority, new_member_priority);

    destroyEventMemberElement(event_member_element);
    destroyMemberPriority(privious_member_priority);
    destroyMemberPriority(new_member_priority);

    return EM_SUCCESS;
}

EventManagerResult emRemoveMemberFromEvent(EventManager em, int member_id, int event_id)
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

    if (getMemberById(em->members, member_id) == NULL)
    {
        return EM_MEMBER_ID_NOT_EXISTS;
    }

    if (!getEventByEventId(em->events, event_id))
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }

    EventsElement event = getEventByEventId(em->events, event_id);

    if (!event)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    MembersElement member = getMemberById(em->members, member_id);

    if (!member)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    EventMemberElement event_member_element = createEventMemberElement(getMemberNameByMember(member), member_id);

    if (!event_member_element)
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    if (!pqContains(getEventMembersByEvent(event), event_member_element))
    {
        destroyEventMemberElement(event_member_element);
        return EM_EVENT_AND_MEMBER_NOT_LINKED;
    }

    pqRemoveElement(getEventMembersByEvent(event), event_member_element);

    int previous_events_counter = getEventsCounterInMember(member);
    subEventsCounterInMember(member);
    int new_events_counter = previous_events_counter - 1;

    MembersPriority privious_member_priority = createMemberPriority(previous_events_counter, member_id);
    MembersPriority new_member_priority = createMemberPriority(new_events_counter, member_id);

    pqChangePriority(em->members, member, privious_member_priority, new_member_priority);

    destroyEventMemberElement(event_member_element);

    return EM_SUCCESS;
}

EventManagerResult emTick(EventManager em, int days)
{
    if (!em)
    {
        return EM_NULL_ARGUMENT;
    }

    if (days <= ZERO)
    {
        return EM_INVALID_DATE;
    }

    dateTickByDays(em->current_date, days);

    if (!removeOccursEvents(em->members, em->events, em->current_date))
    {
        destroyEventManager(em);
        return EM_OUT_OF_MEMORY;
    }

    return EM_SUCCESS;
}

int emGetEventsAmount(EventManager em)
{
    if (!em)
    {
        return NEGATIVE;
    }

    return pqGetSize(em->events);
}

char* emGetNextEvent(EventManager em)
{
    if (!em)
    {
        return NULL;
    }

    return getEventName(pqGetFirst(em->events));

    /*
    if (!(em->is_first_event_returned))
    {
        em->is_first_event_returned = true;
        return getEventName(pqGetFirst(em->events));
    }

    return getEventName(pqGetNext(em->events));
    */
}

void emPrintAllEvents(EventManager em, const char* file_name)
{
    if (!em)
    {
        return;
    }

    FILE* stream = fopen(file_name, "w");

    int day = 0, month = 0, year = 0;

    bool is_first_member_returned = false;

    EventsElement current_event = malloc(sizeof(current_event));

    if (!current_event)
    {
        fclose(stream);
        return;
    }

    EventMembers event_members = malloc(sizeof(event_members));

    if (!event_members)
    {
        destroyEventElement(current_event);
        fclose(stream);
        return;
    }

    current_event = pqGetFirst(em->events);

    while (current_event)
    {
        if (dateGet(getEventDate(current_event), &day, &month, &year))
        {
            fprintf(stream, "%s,%d.%d.%d", getEventName(current_event), day, month, year);
            event_members = getEventMembersByEvent(current_event);

            for (int i = 0; i < pqGetSize(event_members); i++)
            {
                if (!is_first_member_returned)
                {
                    
                    fprintf(stream, ",%s", (char*)getNameByEventMemberElement(pqGetFirst(event_members)));
                    is_first_member_returned = true;
                }
                else
                {
                    fprintf(stream, ",%s", (char*)getNameByEventMemberElement(pqGetNext(event_members)));
                }
            }
            fprintf(stream, "\n");
            is_first_member_returned = false;
            current_event = pqGetNext(em->events);
        }

        else
        {
            destroyEventMembers(event_members);
            destroyEventElement(current_event);
            fclose(stream);
        }
    }

    event_members = NULL;
    destroyEventMembers(event_members);
    destroyEventElement(current_event);

    fclose(stream);
}

void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
    if (!em)
    {
        return;
    }

    FILE* stream = fopen(file_name, "w");
    MembersElement current_member = malloc(sizeof(current_member));

    if (!current_member)
    {
        fclose(stream);
        return;
    }

    current_member = pqGetFirst(em->members);

    while (current_member)
    {
        if (getEventsCounterInMember(current_member) != 0)
        {
            fprintf(stream, "%s,%d\n", (char*)getMemberNameByMember(current_member),
                getEventsCounterInMember(current_member));
        }
        current_member = pqGetNext(em->members);
    }
    
    destroyMembersElement(current_member);
    fclose(stream);
}


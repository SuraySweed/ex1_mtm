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

static void freeElementStructGeneric(PQElement element_struct)
{
    free(((EventsElement)element_struct)->name);
    //free(((EventsElement)element_struct)->id);
    dateDestroy(((EventsElement)element_struct)->date);
    ((EventsElement)element_struct)->event_members = NULL;
    destroyEventMembers(((EventsElement)element_struct)->event_members);
    free(element_struct); //
}

static void freePriorityElementDateGeneric(PQElementPriority date)
{
    dateDestroy(((Date)date));
}

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
    copy_element->name = malloc(sizeof(char) * length + 1);

    if (!copy_element->name)
    {
        free(copy_element);
        return NULL;
    }

    strncpy(copy_element->name, ((EventsElement)element_struct)->name, length + 1);

    copy_element->id = ((EventsElement)element_struct)->id;
    copy_element->date = dateCopy(((EventsElement)element_struct)->date);
    copy_element->event_members = pqCopy(((EventsElement)element_struct)->event_members);

    return copy_element;
}

static PQElementPriority copyPriorityElementDateGeneric(PQElementPriority date)
{
    if (!date)
    {
        return NULL;
    }

    Date copy_date = malloc(sizeof(Date));
    copy_date = dateCopy((Date)date);

    return copy_date;
}

static int compareDatePriorityElementGeneric(PQElementPriority date, PQElementPriority current_date_in_em)
{
    return dateCompare(current_date_in_em, date);
}

static bool equalIdElementGeneric(PQElement element1, PQElement element2)
{
    int is_equal_id = 0;

    is_equal_id = (((EventsElement)element1)->id) - (((EventsElement)element2)->id);

    return is_equal_id == 0 ? true : false;
}

Events createEvents()
{
    Events events = pqCreate(copyStructElementGeneric, freeElementStructGeneric, equalIdElementGeneric,
        copyPriorityElementDateGeneric, freePriorityElementDateGeneric, compareDatePriorityElementGeneric);

    return events;
}

char* getEventName(EventsElement event_element)
{
    if (!event_element)
    {
        return NULL;
    }

    return event_element->name;
}

Date getEventDate(EventsElement event_element)
{
    if (!event_element)
    {
        return NULL;
    }

    return event_element->date;
}

EventsElement getEventByEventId(Events events, int id)
{
    if (pqGetSize(events) == 0)
    {
        return NULL;
    }

    EventsElement event_element = malloc(sizeof(*event_element));
    if (!event_element)
    {
        return NULL;
    }

    event_element = pqGetFirst(events);

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

EventMembers getEventMembersByEvent(EventsElement event)
{
    if (!event)
    {
        return NULL;
    }

    return event->event_members;
}

bool changeDateInElementInEvents(Events events, Date date, int id)
{
    if (!events || !date)
    {
        return false;
    }

    EventsElement found_element = malloc(sizeof(*found_element));
    found_element = ((EventsElement)pqGetFirst(events));

    if (found_element->id == id)
    {
        found_element->date = dateCopy(date);
        return true;
    }

    while (found_element->id != id)
    {
        found_element = ((EventsElement)pqGetNext(events));

        if (found_element->id == id)
        {
            found_element->date = dateCopy(date);
            return true;
        }
    }

    return false;
}

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


    pqChangePriority(members, member_element, member_priority, new_member_priority);
}

bool removeOccursEvents(Members members, Events events, Date current_date)
{
    if (!members || !events || !current_date)
    {
        return false;
    }

    EventsElement current_event = malloc(sizeof(*current_event));

    if (!current_event)
    {
        return false;
    }

    current_event = pqGetFirst(events);

    EventMemberElement event_member_element = malloc(sizeof(event_member_element));

    if (!event_member_element)
    {
        return false;
    }
    
    int member_id = 0;

    while (current_event && dateCompare(current_event->date, current_date) < ZERO)
    {
        event_member_element = pqGetFirst(getEventMembersByEvent(current_event));

        while (event_member_element)
        {
            
            member_id = getIdByEventMemberElement(event_member_element);

            /*
            MembersElement member_element = 
                createMemberElement(getMemberNameByMember(getMemberById(members, member_id)) , member_id, 
                  getEventsCounterInMember(getMemberById(members, member_id)));
            
            MembersPriority member_priority =
                createMemberPriority(getEventsCounterInMember(getMemberById(members, member_id)),
                    getMemberIdByMember(getMemberById(members, member_id)));


            MembersPriority new_member_priority =
                createMemberPriority((getEventsCounterInMember(getMemberById(members, member_id)) - 1),
                    getMemberIdByMember(getMemberById(members, member_id)));


            pqChangePriority(members, member_element, member_priority, new_member_priority);
            */

            changeMemberCounterPriorityWhenRemoveEvent(members, member_id);
            subEventsCounterInMember(getMemberById(members, member_id));

            event_member_element = pqGetNext(getEventMembersByEvent(current_event));

        }
        pqRemove(events);
        current_event = pqGetFirst(events);
    }

    //destroyEventMemberElement(event_member_element);

    return true;
}

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
    event_element->event_members = createEventMembers();

    return event_element;

}

void destroyEventElement(EventsElement event_element)
{
    if (!event_element)
    {
        return;
    }

    freeElementStructGeneric(event_element);
}

void destroyEvents(Events events)
{
    pqDestroy(events);
}

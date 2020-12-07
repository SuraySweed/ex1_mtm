#include "events.h"
#include "priority_queue.h"
#include "date.h"
#include "event_members.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

    strncpy(copy_element->name, ((EventsElement) element_struct)->name, length + 1);
    
    copy_element->id = ((EventsElement)element_struct)->id;
    copy_element->date = dateCopy(((EventsElement) element_struct)->date);
    copy_element->event_members = pqCopy(((EventsElement) element_struct)->event_members);

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

static bool equalStructElementGeneric(PQElement element1, PQElement element2)
{
    //bool is_equal_name = true, is_equal_id = true, is_equal_date = true; // is_equal_event_members;
    bool is_equal_id = true;
    //is_equal_name = !(strcmp(((EventsElement)element1)->name, ((EventsElement)element2)->name));
    is_equal_id = ((EventsElement)element1)->id == ((EventsElement)element2)->id;
    //is_equal_date = dateCompare(((EventsElement)element1)->date, ((EventsElement)element2)->date);

    //return is_equal_name && is_equal_id && !(is_equal_date);
    return is_equal_id;
    /*
    int event1_members_size = pqGetSize(((EventsElement) element1)->event_members);
    int event2_members_size = pqGetSize(((EventsElement) element2)->event_members);

    if(event1_members_size != event2_members_size)
    {
        return false;
    }

    else
    {
        for(int current = 0; current < event1_members_size; current++)
        {
            is_equal_event_members = pqContains( ((EventsElement) element2)->event_members,
        }
    }
    */
}

Events createEvents()
{
    Events events = pqCreate(copyStructElementGeneric, freeElementStructGeneric, equalStructElementGeneric,
        copyPriorityElementDateGeneric, freePriorityElementDateGeneric, compareDatePriorityElementGeneric);
    
    return events;
}

bool changeDateInElementInEvents(Events events, Date date, int id)
{
    if (!events || !date)
    {
        return NULL;
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
    event_element->name = malloc(sizeof(char) * name_length + 1);
    
    if (!(event_element->name))
    {
        freeElementStructGeneric(event_element);
    }
   
    strncpy(event_element->name, name, name_length + 1);
    event_element->id = id;
    event_element->date = dateCopy(date);
    event_element->event_members = pqCopy(event_members);

    return event_element;

}

void destroyEvents(Events events)
{
    pqDestroy(events);
}


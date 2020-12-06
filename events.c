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

    bool is_found = true;

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



void destroyEvents(Events events)
{
    pqDestroy(events);
}


int main()
{
    Events events = createEvents();

    EventsElement element1 = malloc(sizeof(*element1));
    EventsElement element2 = malloc(sizeof(*element2));
    EventsElement element3 = malloc(sizeof(*element3));
    EventsElement element4 = malloc(sizeof(*element3));


    char* name1 = "technion party";
    int id1 = 1;
    Date date1 = dateCreate(29, 10, 2020);
    element1->id = id1;
    element1->date = dateCopy(date1);
    element1->name = malloc(strlen(name1) + 1);
    strcpy(element1->name, name1);
    element1->event_members = NULL;

    char* name2 = "student day";
    int id2 = 2;
    Date date2 = dateCreate(12, 12, 2020);
    element2->id = id2;
    element2->name = malloc(strlen(name2) + 1);
    strcpy(element2->name, name2);
    element2->date = dateCopy(date2);
    element2->event_members = NULL;

    char* name3 = "exams day";
    int id3 = 3;
    Date date3 = dateCreate(2, 2, 2021);
    element3->id = id3;
    element3->date = dateCopy(date3);
    element3->name = malloc(strlen(name3) + 1);
    strcpy(element3->name, name3);
    element3->event_members = NULL;

    pqInsert(events, element1, date1);
    pqInsert(events, element2, date2);
    pqInsert(events, element3, date3);


    EventsElement check_element1 = malloc(sizeof(*check_element1));
    EventsElement check_element2 = malloc(sizeof(*check_element2));
    EventsElement check_element3 = malloc(sizeof(*check_element3));
    EventsElement check_element4 = malloc(sizeof(*check_element4));


    int day = 0, month = 0, year = 0;

    check_element1 = ((EventsElement)pqGetFirst(events));
    check_element2 = ((EventsElement)pqGetNext(events));
    check_element3 = ((EventsElement)pqGetNext(events));


    printf("-----inserting testing-----\n");
    printf("the name of the first element event is : %s\n", check_element1->name);
    printf("the id of the first element event is : %d\n", check_element1->id);
    dateGet(check_element1->date, &day, &month, &year);
    printf("the date of the first element is: %d %d %d:\n", day, month, year);

    printf("\nthe name of the second element event is : %s\n", check_element2->name);
    printf("the name of the second element event is :\n", check_element2->id);
    dateGet(check_element2->date, &day, &month, &year);
    printf("the date of the second element is: %d.%d.%d:\n", day, month, year);

    printf("\nthe name of the 3rd element event is : %s\n", check_element3->name);
    printf("the id of the 3rd element event is : %d\n", check_element3->id);
    dateGet(check_element3->date, &day, &month, &year);
    printf("the date of the 3rd element is: %d.%d.%d:\n", day, month, year);

    Date date_change = dateCreate(1, 1, 2019);

    EventsElement check_change_element = malloc(sizeof(*check_change_element));

    pqChangePriority(events, element2, date2, date_change);
    check_change_element = ((EventsElement)pqGetFirst(events));
    
    bool i = changeDateInElementInEvents(events, date_change, element2->id);
    
    
    printf("\n-----after changing------\n");
    printf("the name of the first element event is : %s\n", check_change_element->name);
    printf("the id of the first element event is : %d\n", check_change_element->id);
    dateGet(check_change_element->date, &day, &month, &year);
    printf("the date of the first element is: %d.%d.%d\n", day, month, year);


    check_change_element = ((EventsElement)pqGetNext(events));

    printf("the name of the first element event is : %s\n", check_change_element->name);
    printf("the id of the first element event is : %d\n", check_change_element->id);
    dateGet(check_change_element->date, &day, &month, &year);
    printf("the date of the first element is: %d.%d.%d\n", day, month, year);


    check_change_element = ((EventsElement)pqGetNext(events));

    printf("the name of the first element event is : %s\n", check_change_element->name);
    printf("the id of the first element event is : %d\n", check_change_element->id);
    dateGet(check_change_element->date, &day, &month, &year);
    printf("the date of the first element is: %d.%d.%d\n", day, month, year);


    printf("\n------after removing-----\n");
    pqRemove(events);
    check_change_element = ((EventsElement)pqGetFirst(events));
    printf("the name of the first element event is : %s\n", check_change_element->name);
    printf("the id of the first element event is : %d\n", check_change_element->id);
    dateGet(check_change_element->date, &day, &month, &year);
    printf("the date of the first element is: %d.%d.%d\n", day, month, year);

    printf("\n------remove by element------\n");
    pqRemoveElement(events, element1);
    check_change_element = ((EventsElement)pqGetFirst(events));
    printf("the name of the first element event is : %s\n", check_change_element->name);
    printf("the id of the first element event is : %d\n", check_change_element->id);
    dateGet(check_change_element->date, &day, &month, &year);
    printf("the date of the first element is: %d.%d.%d\n", day, month, year);

    printf("\n-----after clearing-------\n");
    pqClear(events);
    check_change_element = ((EventsElement)pqGetFirst(events));
    printf("this: %s\n", check_change_element);

    printf("\n----change priority while the queue is null------\n");
    
    if (pqChangePriority(events, element1, date1, date2) == 3)
    {
        printf("PQ NULL ARGUMENT\n");
    }


    /*
    free(date1);
    free(date2);
    free(date3);
    free(date_change);

    free(element1);
    free(element2);
    free(element3);
    free(element4);
    
    free(check_element1);
    free(check_element2);
    free(check_element3);
    free(check_change_element);
    */

    destroyEvents(events);

    return 0;
}
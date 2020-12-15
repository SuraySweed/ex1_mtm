#include "event_members.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "priority_queue.h" 

struct EventMemberElement_t {
    char* name;
    int id;
};

static void freeElementStructGeneric(PQElement element_struct)
{
    if (!element_struct)
    {
        return;
    }

    free(((EventMemberElement) element_struct)->name);
    free(element_struct);
}

static void freePriorityElementIntGeneric(PQElementPriority id)
{
    free(id);
}

static PQElement copyStructElementGeneric(PQElement element_struct)
{
    if (!element_struct)
    {
        return NULL;
    }

    int name_size = strlen(((EventMemberElement)element_struct)->name);

    EventMemberElement copy_element = malloc(sizeof(*copy_element));

    if (!copy_element)
    {
        return NULL;
    }

    copy_element->name = malloc((sizeof(char) * name_size) + 1);

    if (!copy_element->name)
    {
        freeElementStructGeneric(element_struct);
        return NULL;
    }

    strncpy(copy_element->name, ((EventMemberElement)element_struct)->name, name_size + 1);
    copy_element->id = ((EventMemberElement)element_struct)->id;


    return copy_element;
}

static PQElementPriority copyIntPriorityElementGeneric(PQElementPriority id)
{
    if (!id)
    {
        return NULL;
    }

    int* copy = malloc(sizeof(*copy));
    
    if (!copy)
    {
        return NULL;
    }
    
    *copy = *(int*)id;

    return copy;
}

static int compareIntPriorityElementGeneric(PQElementPriority id1, PQElementPriority id2)
{
    return(*(int*)id2 - *(int*)id1);
}

static bool equalIdElementsGeneric(PQElement element1, PQElement element2)
{
    return (((EventMemberElement)element1)->id == ((EventMemberElement)element2)->id);
}

EventMembers createEventMembers()
{
    EventMembers event_members = pqCreate(copyStructElementGeneric, freeElementStructGeneric,
        equalIdElementsGeneric, copyIntPriorityElementGeneric, freePriorityElementIntGeneric,
        compareIntPriorityElementGeneric);

    return event_members;
}

EventMemberElement createEventMemberElement(char* name, int id)
{
    if (!name)
    {
        return NULL;
    }

    EventMemberElement build_element = malloc(sizeof(*build_element));

    if (!build_element)
    {
        return NULL;
    }

    int name_length = strlen(name);

    build_element->name = malloc(sizeof(char) * name_length + 1);
    strcpy(build_element->name, name);

    build_element->id = id;

    return build_element;
}

int getIdByEventMemberElement(EventMemberElement event_member_element)
{
    return event_member_element->id;
}

char* getNameByEventMemberElement(EventMemberElement event_members_element)
{
    if (!event_members_element)
    {
        return NULL;
    }
    
    return event_members_element->name;
}


void destroyEventMemberElement(EventMemberElement event_members_element)
{
    if (!event_members_element)
    {
        return;
    }

    freeElementStructGeneric(event_members_element);
}

void destroyEventMembers(EventMembers event_members)
{
    pqDestroy(event_members);
}

#include "event_members.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "priority_queue.h" 
#include <string.h>

static void freeElementStringGeneric(PQElement str)
{
    free(str);
}

static void freePriorityElementIntGeneric(PQElementPriority id)
{
    free(id);
}

static PQElement copyStringElementGeneric(PQElement str)
{
    if (!str)
    {
        return NULL;
    }

    char* copy_str = malloc((sizeof(*copy_str) * strlen(str)) + 1);

    if (!copy_str)
    {
        return NULL;
    }

    strncpy(copy_str, str, strlen(str) + 1);

    return copy_str;
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
    return(*(int*)id1 - *(int*)id2);
}

static bool equalStringsElementsGeneric(PQElement str1, PQElement str2)
{
    return (!(strcmp(str1, str2)));
}

EventMembers createEventMembers()
{
    EventMembers event_members = pqCreate(copyStringElementGeneric, freeElementStringGeneric,
        equalStringsElementsGeneric, copyIntPriorityElementGeneric, freePriorityElementIntGeneric,
        compareIntPriorityElementGeneric);

    return event_members;
}

void destroyEventMembers(EventMembers event_members)
{
    pqDestroy(event_members);
}

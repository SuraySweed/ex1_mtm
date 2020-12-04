#include "event_members.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "priority_queue.h" 

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


int main()
{
    char* str = "suray";
    int id1 = 2;

    char* str2 = "boran";
    int id2 = 5;

    PriorityQueue event_member = createEventMembers();
   

    pqInsert(event_member, str, &id1);
    pqInsert(event_member, str2, &id2);


    printf("the first name in the queue is: %s\n", pqGetFirst(event_member));
    printf("the next name is : %s\n", pqGetNext(event_member));
    printf("the size is: %d\n----------\n", pqGetSize(event_member));
    int newID2 = 1;

    pqChangePriority(event_member, str2, &id2, &newID2);

    printf("after change priority:\n---------\n");
    printf("the first name in the queue is: %s\n", pqGetFirst(event_member));
    printf("the next name is : %s\n", pqGetNext(event_member));
    printf("the size is: %d\n----------\n", pqGetSize(event_member));


    pqRemove(event_member);
    printf("---------\nafter removing:\n----------\n");
    printf("the first name is:%s\n", pqGetFirst(event_member));
    printf("the size is: %d\n----------\n", pqGetSize(event_member));

    printf("--------\ninsert testing--------\n");



    int id3 = 8;
    int id4 = 8;
    char* name3 = "gogo";
    char* name4 = "loffy";

    pqInsert(event_member, name3, &id3);
    pqInsert(event_member, name4, &id4);

    printf("the first name in the queue is: %s\n", pqGetFirst(event_member));
    printf("the next name is : %s\n", pqGetNext(event_member));
    printf("the next name is : %s\n", pqGetNext(event_member));

    pqClear(event_member);
    
    printf("-----------after clearing----------\n");
    printf("the first name in the queue is: %s\n", pqGetFirst(event_member));

    int id5 = 5;
    char* name5 = "konan";

    pqInsert(event_member, name5, &id5);
    printf("the first name in the queue is: %s\n", pqGetFirst(event_member));
    pqRemoveElement(event_member, name5);
    printf("the first name in the queue is: %s\n", pqGetFirst(event_member));
    

    printf("------change priority for unexist element------\n");
    printf("the first name in the queue is: %s\n", pqGetFirst(event_member));

    pqRemove(event_member);
    printf("the first name in the queue is: %s\n", pqGetFirst(event_member));

    destroyEventMembers(event_member);


    return 0;
}
#include "members.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include"priority_queue.h"
#include <string.h>

struct MembersElement_t {
	char* name;
	int id;
	int events_counter;
};


static void freeMembersElementsStructGeneric(PQElement element_struct)
{
	free(((MembersElement)element_struct)->name);
	//free(((MembersElement)element_struct)->id);
	//free(((MembersElement)element_struct)->events_counter);
	free(element_struct);
}

static void freeMembersPriorityIntGeneric(PQElementPriority events_counter)
{
	free(events_counter);
}

static PQElement copyMembersElementStructGeneric(PQElement element_struct)
{
	if (!element_struct)
	{
		return NULL;
	}

	MembersElement copy_element = malloc((sizeof(*copy_element)));

	if (!copy_element)
	{
		return NULL;
	}
	copy_element->name = malloc((sizeof(char) * (strlen(((MembersElement)element_struct)->name)) + 1));
	if (!copy_element->name)
	{
		freeMembersElementsStructGeneric(copy_element);
		return NULL;
	}
	strcpy(copy_element->name, ((MembersElement)element_struct)->name);
	copy_element->id = (((MembersElement)element_struct)->id);
	copy_element->events_counter = (((MembersElement)element_struct)->events_counter);

	return copy_element;
}

static PQElementPriority copyElementPriorityIntGeneric(PQElementPriority element_priority)
{
	if (!element_priority)
	{
		return NULL;
	}

	int* copy_element_priority = malloc(sizeof(*copy_element_priority));

	if (!copy_element_priority)
	{
		return NULL;
	}

	*copy_element_priority = *(int*)element_priority;

	return copy_element_priority;
}

static bool equalMembersElementsStructGeneric(PQElement member1_element, PQElement member2_element)
{
	//bool is_equal_name = !strcmp(((MembersElement)member1_element)->name, ((MembersElement)member2_element)->name);
	bool is_equal_id = ((MembersElement)member1_element)->id == ((MembersElement)member2_element)->id;
	//int events_counter_compare = ((MembersElement)member1_element)->events_counter - ((MembersElement)member2_element)->events_counter;

	return (is_equal_id); //&& is_equal_id);//&& !events_counter_compare)
}

static int compareMembersPriorityIntGeneric(PQElementPriority member1_priority, PQElementPriority member2_priority)
{
	int priority_compare = *(int*)member1_priority - *(int*)member2_priority;

	return priority_compare;
}



Members createMembers()
{
	Members members = pqCreate(copyMembersElementStructGeneric,
		freeMembersElementsStructGeneric,
		equalMembersElementsStructGeneric,
		copyElementPriorityIntGeneric,
		freeMembersPriorityIntGeneric,
		compareMembersPriorityIntGeneric);

	return members;
}

MembersElement createMemberElement(char* name, int id, int events_counter)
{
	if (!name)
	{
		return NULL;
	}

	MembersElement build_element = malloc(sizeof(*build_element));

	if (!build_element)
	{
		return NULL;
	}

	int length = strlen(name);
	
	build_element->name = malloc(sizeof(char*) * length + 1);
	strcpy(build_element->name, name);
	
	build_element->id = id;
	build_element->events_counter = events_counter;

	return build_element;
}

bool changeCounterInElement(Members members, int id, int new_counter)
{
	if (!members)
	{
		return NULL;
	}

	MembersElement found_element = malloc(sizeof(*found_element));
	found_element = ((MembersElement)pqGetFirst(members));

	if (found_element->id == id)
	{
		found_element->events_counter = new_counter;
		return true;
	}

	while (found_element->id != id)
	{
		found_element = ((MembersElement)pqGetNext(members));

		if (found_element->id == id)
		{
			found_element->events_counter = new_counter;
			return true;
		}
	}

	return false;
}

MembersElement getMemberById(Members members, int id)
{
	if (!members)
	{
		return NULL;
	}

	MembersElement member_element = malloc(sizeof(*member_element));

	if (!member_element)
	{
		return NULL;
	}

	member_element = pq(pqGetFirst(members));

	if (member_element->id == id)
	{
		return member_element;
	}

	while (member_element->id != id)
	{
		member_element = pqGetNext(members);
		if (member_element->id == id)
		{
			return member_element;
		}
	}

	return NULL;

}

void  destroyMembers(Members members)
{
	pqDestroy(members);
}

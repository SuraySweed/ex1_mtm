#include "members.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include"priority_queue.h"
#include <string.h>

#define ZERO 0
#define NEGATIVE -1
#define POSITIVE 1

struct MembersElement_t {
	char* name;
	int id;
	int events_counter;
};

struct MembersPriority_t {
	int events_counter;
	int member_id;
};


static void freeMembersElementsStructGeneric(PQElement element_struct)
{
	free(((MembersElement)element_struct)->name);
	//free(((MembersElement)element_struct)->id);
	//free(((MembersElement)element_struct)->events_counter);
	free(element_struct);
}

static void freeMembersPriorityStructGeneric(PQElementPriority priority_struct)
{
	//free(((MembersPriority)priority_struct)->events_counter);
	//free(((MembersPriority)priority_struct)->member_id);
	free(priority_struct);
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
	copy_element->name = malloc(((sizeof(char) * (strlen(((MembersElement)element_struct)->name))) + 1));
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

static PQElementPriority copyElementPriorityStructGeneric(PQElementPriority element_priority)
{
	if (!element_priority)
	{
		return NULL;
	}

	MembersPriority copy_member_priority = malloc(sizeof(*copy_member_priority));

	if (!copy_member_priority)
	{
		return NULL;
	}

	copy_member_priority->events_counter = ((MembersPriority)element_priority)->events_counter;
	copy_member_priority->member_id = ((MembersPriority)element_priority)->member_id;

	return copy_member_priority;
}

static bool equalMembersElementsStructGeneric(PQElement member1_element, PQElement member2_element)
{
	//bool is_equal_name = !strcmp(((MembersElement)member1_element)->name, ((MembersElement)member2_element)->name);
	bool is_equal_id = ((MembersElement)member1_element)->id == ((MembersElement)member2_element)->id;
	//int events_counter_compare = ((MembersElement)member1_element)->events_counter - ((MembersElement)member2_element)->events_counter;

	return (is_equal_id); //&& is_equal_id);//&& !events_counter_compare)
}

static int compareMembersPriorityGeneric(PQElementPriority member1_priority, PQElementPriority member2_priority)
{
	if (((MembersPriority)member1_priority)->events_counter - ((MembersPriority)member2_priority)->events_counter > ZERO)
	{
		return POSITIVE;
	}
	
	else if ( (((MembersPriority)member2_priority)->events_counter) -
		(((MembersPriority)member1_priority)->events_counter) == ZERO)
	{
		return ((MembersPriority)member2_priority)->member_id -
			((MembersPriority)member1_priority)->member_id;
	}
	else
	{
		return NEGATIVE;
	}
}


Members createMembers()
{
	Members members = pqCreate(copyMembersElementStructGeneric,
		freeMembersElementsStructGeneric,
		equalMembersElementsStructGeneric,
		copyElementPriorityStructGeneric,
		freeMembersPriorityStructGeneric,
		compareMembersPriorityGeneric);

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

MembersPriority createMemberPriority(int events_counter, int member_id)
{
	MembersPriority build_priority = malloc(sizeof(*build_priority));

	if (!build_priority)
	{
		return NULL;
	}

	build_priority->events_counter = events_counter;
	build_priority->member_id = member_id;

	return build_priority;
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
	if (pqGetSize(members) == 0) 
	{
		return NULL;
	}

	MembersElement member_element = malloc(sizeof(*member_element));

	if (!member_element)
	{
		return NULL;
	}

	member_element = pqGetFirst(members);

	if (member_element->id == id)
	{
		return member_element;
	}

	while (member_element && member_element->id != id)
	{
		member_element = pqGetNext(members);
		if (member_element && member_element->id == id)
		{
			return member_element;
		}
	}
	return NULL;
}

char* getMemberNameByMember(MembersElement member)
{
	if (!member)
	{
		return NULL;
	}

	return member->name;
}

int getEventsCounterInMember(MembersElement member)
{
	if (!member)
	{
		return 0;
	}

	return member->events_counter;
}

int getMemberIdByMember(MembersElement member)
{
	return member->id;
}

void addEventsCounterInMember(MembersElement member)
{
	if (!member)
	{
		return;
	}

	member->events_counter = member->events_counter + 1;
}

void subEventsCounterInMember(MembersElement member)
{
	if (!member)
	{
		return;
	}

	member->events_counter = member->events_counter - 1;
}

void destroyMembersElement(MembersElement members_element)
{
	if (!members_element)
	{
		return;
	}

	freeMembersElementsStructGeneric(members_element);
}

void destroyMemberPriority(MembersPriority member_priority)
{
	free(member_priority);
}

void  destroyMembers(Members members)
{
	pqDestroy(members);
}

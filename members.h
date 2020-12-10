#ifndef MEMBERS_H
#define MEMBERS_H

#include "priority_queue.h"

typedef PriorityQueue Members;
typedef struct MembersElement_t* MembersElement;

Members createMembers();

bool changeCounterInElement(Members members, int id, int new_counter);
MembersElement getMemberById(Members members, int id);

MembersElement createMemberElement(char* name, int id, int events_counter);

void destroyMembers(Members members);


#endif /* MEMBERS_H_ */

#ifndef MEMBERS_H
#define MEMBERS_H

#include "priority_queue.h"

typedef PriorityQueue Members;
typedef struct MembersElement_t* MembersElement;
typedef struct MembersPriority_t* MembersPriority;


Members createMembers();

bool changeCounterInElement(Members members, int id, int new_counter);
MembersElement getMemberById(Members members, int id);
char* getMemberNameByMember(MembersElement member);
int getEventsCounterInMember(MembersElement member);
int getMemberIdByMember(MembersElement member);

void addEventsCounterInMember(MembersElement member);
void subEventsCounterInMember(MembersElement member);

MembersElement createMemberElement(char* name, int id, int events_counter);
MembersPriority createMemberPriority(int events_counter, int member_id);

void destroyMembersElement(MembersElement members_element);
void destroyMemberPriority(MembersPriority member_priority);
void destroyMembers(Members members);


#endif /* MEMBERS_H_ */

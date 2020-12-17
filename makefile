event_manager: event_manager.o event_members.o date.o events.o members.o priority_queue.o event_manager_tests.o
	gcc -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG event_manager.o event_members.o date.o events.o members.o priority_queue.o event_manager_tests.o -o event_manager

priority_queue: priority_queue.o pq_tests.o
	gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG priority_queue.o pq_tests.o -o priority_queue

event_manager.o: event_manager.c event_manager.h priority_queue.h date.h events.h members.h event_members.h
	gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -c event_manager.c

event_members.o: event_members.c event_members.h priority_queue.h
	gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -c event_members.c

events.o: events.c events.h priority_queue.h date.h event_members.h 
	gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -c events.c

date.o: date.c date.h
	gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -c date.c

members.o: members.c members.h priority_queue.h
	gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -c members.c

event_manager_tests.o: tests/event_manager_tests.c tests/test_utilities.h event_manager.h
	gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -c tests/event_manager_tests.c

priority_queue.o: priority_queue.c priority_queue.h
	gcc  -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -c priority_queue.c

pq_tests.o: tests/pq_tests.c tests/test_utilities.h priority_queue.h
	gcc -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -c tests/pq_tests.c

clean:
	rm -f *.o priority_queue event_manager



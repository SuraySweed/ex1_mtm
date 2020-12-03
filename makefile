prog priority_queue.o
	gcc priority_queue.o -o prog

priority_queue.o: priority_queue.h
	gcc -c priority_queue.c



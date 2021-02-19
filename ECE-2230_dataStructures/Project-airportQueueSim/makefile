c_qDum: queue.c
	gcc -Wall queue.c dumMain.c

c_pqDum: priority.c
	gcc -Wall priority.c dumMain.c

c_pq_qDum: priority.c queue.c
	gcc -Wall priority.c queue.c dumMain.c

eventDum: event.c
	gcc event.c priority.c dumMain.c

realmain: main.c priority.c event.c queue.c time.c randsim.c
	gcc -Wall -o P2 main.c priority.c event.c queue.c time.c randsim.c -lm

tests: queuetests.c priority.c event.c queue.c time.c randsim.c
	gcc -Wall -o test1 queuetests.c priority.c event.c queue.c time.c randsim.c -lm
run: P2
	./P2 -e 500


BINARIES :=  threadHello TSQueueMain BBQMain RWLockMain BarberShopMain
all: $(BINARIES)

CFLAGS := $(CFLAGS) -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS

THREADLIBS := Lock.o CV.o thread.o
CTHREADLIBS := thread.o

clean:
	rm -f *.o $(BINARIES)

tags:
	etags *.h *.c *.cc

%.o: %.cc
	g++ -c $(CFLAGS) $< -o $@

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

threadHello: threadHello.c $(CTHREADLIBS)
	gcc $(CFLAGS) $^ -o $@

bzero: bzero.c $(CTHREADLIBS)
	gcc $(CFLAGS) $^ -o $@

TSQueueMain: TSQueueMain.o TSQueue.o $(THREADLIBS)
	g++ $(CFLAGS) $^ -o $@

RWLockMain: RWLockMain.o RWLock.o RWObj.o $(THREADLIBS)
	g++ $(CFLAGS) $^ -o $@

BBQMain: BBQMain.o BBQ.o $(THREADLIBS)
	g++ $(CFLAGS) $^ -o $@

BarberShopMain: BarberShopMain.o BarberShop.o $(THREADLIBS)
	g++ $(CFLAGS) $^ -o $@

CounterMain: CounterMain.o Counter.o $(THREADLIBS)
	g++ $(CFLAGS) $^ -o $@


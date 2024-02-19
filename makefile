CC = gcc
CFLAGS = -Wall -fsanitize=address -std=c99
# DEBUG = -DDEBUG

memtestex: memtest.o mymalloc.o
	$(CC) $(CFLAGS) $(DEBUG) memtest.o mymalloc.o -o memtestex

mymalloc.o: mymalloc.c mymalloc.h  
	$(CC) $(CFLAGS) $(DEBUG) -c -Wall mymalloc.c  

memtest.o: memtest.c mymalloc.h  
	$(CC) $(CFLAGS) $(DEBUG) -c -Wall memtest.c

memtest: memtestex
	./memtestex

memgrind: memgrindex
	./memgrindex

memgrindex: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) $(DEBUG) memgrind.o mymalloc.o -o memgrindex

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) $(DEBUG) -c -Wall memgrind.c

clean:
	rm *.o memtestex memgrindex

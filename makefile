CC = gcc
CFLAGS = -Wall -fsanitize=address -std=c99
DEBUG = -DDEBUG

memtest: memtest.o mymalloc.o  
	$(CC) $(CFLAGS) $(DEBUG) memtest.o mymalloc.o -o memtest  

mymalloc.o: mymalloc.c mymalloc.h  
	$(CC) $(CFLAGS) $(DEBUG) -c -Wall mymalloc.c  

memtest.o: memtest.c mymalloc.h  
	$(CC) $(CFLAGS) $(DEBUG) -c -Wall memtest.c

testmem: memtest
	./memtest

testmy: mytest
	./mytest

mytest: mytest.o mymalloc.o
	$(CC) $(CFLAGS) $(DEBUG) mytest.o mymalloc.o -o mytest

mytest.o: mytest.c mymalloc.h
	$(CC) $(CFLAGS) $(DEBUG) -c -Wall mytest.c

cleanmem:  
	rm *.o memtest

cleanmy:
	rm *o mytest

CC = gcc
CFLAGS = -Wall -I./src

all: scheduler

scheduler: src/main.o src/scheduler.o
	$(CC) src/main.o src/scheduler.o -o scheduler

src/main.o: src/main.c src/scheduler.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/scheduler.o: src/scheduler.c src/scheduler.h
	$(CC) $(CFLAGS) -c src/scheduler.c -o src/scheduler.o

clean:
	rm -f src/*.o scheduler

CC = gcc
INC = -I.
FLAGS = -pthread -lncurses

all: tests.o board.o ultimate.c game.o queue.o computer.o evaluation.o
	$(CC) $(FLAGS) $^ -o ultimate

board.o: board.c board.h
	$(CC) -c board.c
tests.o: tests.c tests.h board.o queue.o computer.o evaluation.o
	$(CC) -c tests.c
game.o: game.c game.h board.o computer.o
	$(CC) -c game.c
queue.o: queue.c queue.h
	$(CC) -c queue.c
computer.o: board.o queue.o computer.c computer.h evaluation.o
	$(CC) -c computer.c
evaluation.o: board.o evaluation.c evaluation.h
	$(CC) -c evaluation.c

clean:
	-rm *.o
	-rm ultimate

CC = gcc
CFLAGS = -Wall
LIBS = -lcurl -lcjson

all: monitor

monitor: main.o
	$(CC) -o monitor main.o $(LIBS)

main.o: main.c monitoring_system.h
	$(CC) $(CFLAGS) -c main.c $(LIBS)

clean:
	rm -f *.o monitor

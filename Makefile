CC=g++
CFLAGS=-c -Wall

all: ad-task.out

ad-task.out: main.o figure.o
	$(CC) main.o figure.o -o ad-task.out -pthread

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

figure.o: figure.cpp
	$(CC) $(CFLAGS) figure.cpp

clean:
	rm -rf *.o ad-task



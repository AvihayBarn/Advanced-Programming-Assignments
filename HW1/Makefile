CC=gcc

all: coder


coder: thpool.o main.o
	gcc -o coder thpool.o main.o ./libCodec.so



main.o: main.c thpool.h
	gcc -c main.o main.c



thpool.o: thpool.c thpool.h
	gcc -c thpool.o thpool.c



clean:
	rm -rf *.o coder
CC=gcc
CFLAGS=-Wall -Werror -pthread -std=c99

main: main.o
	$(CC) ${CFLAGS} -o main main.o

run: main
    ./main
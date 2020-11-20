CC=gcc
CFLAGS= -I -g -Wall -Wextra -ggdb3
DEPS = ../Libraries/file_processor.h ../Libraries/file_processor.c ../Libraries/JackWendyConnections.c ../Libraries/JackWendyConnections.h ../Libraries/frame.c ../Libraries/frame.h
OBJ = main.o ../Libraries/file_processor.o ../Libraries/JackWendyConnections.o ../Libraries/frame.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Jack: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	-rm edit $(objects)

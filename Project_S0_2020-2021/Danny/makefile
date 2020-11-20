CC=gcc
CFLAGS= -I -g -Wall -Wextra -ggdb3
DEPS = ../Libraries/file_processor.h ../Libraries/file_directory.c ../Libraries/DannyConnections.h ../Libraries/DannyConnections.c ../Libraries/frame.c ../Libraries/frame.h
OBJ = main.o ../Libraries/file_processor.o ../Libraries/DannyConnections.o ../Libraries/frame.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Danny: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	-rm edit $(objects)

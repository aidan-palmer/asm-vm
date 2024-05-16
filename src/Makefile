CC = gcc
CFLAGS = -Wall -g
SOURCES = vm.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = vm

all: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) -lm

run: all
	./vm test.s

clean:
	$(RM) $(OBJECTS) $(TARGET)

hello: all
	./vm ../tests/hello-world.s

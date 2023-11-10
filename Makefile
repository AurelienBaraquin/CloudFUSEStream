CC = gcc
CFLAGS = -Wall $(shell pkg-config fuse3 --cflags) -Iinclude
LDFLAGS = $(shell pkg-config fuse3 --libs) -lz

SRCDIR = src
C_SOURCES = $(shell find $(SRCDIR) -name '*.c')
C_OBJECTS = $(C_SOURCES:.c=.o)
OBJECTS = $(C_OBJECTS)
EXECUTABLE = CFSTREAM

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(C_OBJECTS) $(EXECUTABLE)

re: clean all

.PHONY: all run clean re

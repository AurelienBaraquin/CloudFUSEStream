CC = gcc
CFLAGS = -Wall -g3 -c -DNDEBUG=1 $(shell pkg-config fuse3 --cflags) -Iinclude
LDFLAGS = $(shell pkg-config fuse3 --libs) -lz

SRCDIR = src
C_SOURCES = $(shell find $(SRCDIR) -name '*.c')
C_OBJECTS = $(C_SOURCES:.c=.o)
OBJECTS = $(C_OBJECTS)
EXECUTABLE = CFStream

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

host: $(EXECUTABLE)
	./$(EXECUTABLE) host 6567

connect: $(EXECUTABLE)
	mkdir -p rep
	./$(EXECUTABLE) connect 127.0.0.1 6567 rep

clean:
	rm -f $(C_OBJECTS) $(EXECUTABLE)

re: clean all

.PHONY: all host clean re

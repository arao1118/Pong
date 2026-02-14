# Get source file name without extension
SRC := $(wildcard *.c)
TARGET := $(basename $(SRC))

CC := gcc
CFLAGS := -g -Wall -Wextra -std=c11
LDFLAGS := -lSDL3

all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

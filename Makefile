CC = gcc
CFLAGS = -std=c89 -Wall -g
SRCS = main.c processor.c memory.c
OBJS = $(SRCS:.c=.o)
TARGET = svm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c svm.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..

objects := priority_queue_client

# Priority Queue Client
priority_queue_client: lib.o priority_queue_client.o priority_queue.o
	@echo Compiling priority_queue_client
	$(CC) $(CFLAGS) $^ -o $@

# Common library
lib.o: $(ALGORITHMS_PATH)/lib/lib.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) *.o
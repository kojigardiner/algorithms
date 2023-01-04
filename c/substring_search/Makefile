CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..

objects := substring_search_client

all: $(objects)

substring_search_client: lib.o substring_search_client.o substring_search.o
	@echo Compiling substring_search_client
	$(CC) $(CFLAGS) $^ -o $@

substring_search.o: $(ALGORITHMS_PATH)/substring_search/substring_search.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Common library
lib.o: $(ALGORITHMS_PATH)/lib/lib.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) *.o
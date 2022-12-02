CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..

objects := paths_client

all: $(objects)

paths_client: lib.o paths_client.o symbol_table.o symbol_graph.o graph.o paths.o stack.o queue.o bag.o
	@echo Compiling paths_client
	$(CC) $(CFLAGS) $^ -o $@

symbol_table.o: $(ALGORITHMS_PATH)/symbol_table/symbol_table.c
	$(CC) -c $(CFLAGS) $^ -o $@

graph.o: $(ALGORITHMS_PATH)/graph/graph.c
	$(CC) -c $(CFLAGS) $^ -o $@

stack.o: $(ALGORITHMS_PATH)/stack/stack.c
	$(CC) -c $(CFLAGS) $^ -o $@

queue.o: $(ALGORITHMS_PATH)/queue/queue.c
	$(CC) -c $(CFLAGS) $^ -o $@

bag.o: $(ALGORITHMS_PATH)/bag/bag.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Common library
lib.o: $(ALGORITHMS_PATH)/lib/lib.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) *.o
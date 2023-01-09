CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..

objects := zip_client

all: $(objects)

zip_client: lib.o zip_client.o zip.o bit_io.o priority_queue.o symbol_table.o stack.o queue.o
	@echo Compiling zip_client
	$(CC) $(CFLAGS) $^ -o $@

substring_search.o: $(ALGORITHMS_PATH)/zip/zip.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Common library
lib.o: $(ALGORITHMS_PATH)/lib/lib.c
	@$(CC) -c $(CFLAGS) $^ -o $@

bit_io.o: $(ALGORITHMS_PATH)/bit_io/bit_io.c
	@$(CC) -c $(CFLAGS) $^ -o $@

priority_queue.o: $(ALGORITHMS_PATH)/priority_queue/priority_queue.c
	@$(CC) -c $(CFLAGS) $^ -o $@

symbol_table.o: $(ALGORITHMS_PATH)/symbol_table/symbol_table.c
	@$(CC) -c $(CFLAGS) $^ -o $@

stack.o: $(ALGORITHMS_PATH)/stack/stack.c
	@$(CC) -c $(CFLAGS) $^ -o $@

queue.o: $(ALGORITHMS_PATH)/queue/queue.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) *.o
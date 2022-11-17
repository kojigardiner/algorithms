CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..

objects := symbol_table_client st_worst_case_client

all: $(objects)

symbol_table_client: lib.o symbol_table_client.o symbol_table.o stack.o
	@echo Compiling symbol_table_client
	$(CC) $(CFLAGS) $^ -o $@

st_worst_case_client: lib.o st_worst_case_client.o symbol_table.o stack.o
	@echo Compiling st_worst_case_client
	$(CC) $(CFLAGS) $^ -o $@

stack.o: $(ALGORITHMS_PATH)/stack/stack.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Common library
lib.o: $(ALGORITHMS_PATH)/lib/lib.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) *.o
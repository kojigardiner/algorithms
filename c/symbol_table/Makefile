CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..

objects := symbol_table_client st_worst_case_client st_movies st_autocomplete

all: $(objects)

st_autocomplete: lib.o st_autocomplete.o symbol_table.o stack.o queue.o 
	@echo Compiling st_autocomplete
	$(CC) $(CFLAGS) $^ -o $@

st_movies: lib.o st_movies.o symbol_table.o stack.o queue.o 
	@echo Compiling st_movies
	$(CC) $(CFLAGS) $^ -o $@

symbol_table_client: lib.o symbol_table_client.o symbol_table.o stack.o queue.o 
	@echo Compiling symbol_table_client
	$(CC) $(CFLAGS) $^ -o $@

st_worst_case_client: lib.o st_worst_case_client.o symbol_table.o stack.o queue.o 
	@echo Compiling st_worst_case_client
	$(CC) $(CFLAGS) $^ -o $@

symbol_table.o: $(ALGORITHMS_PATH)/symbol_table/symbol_table.c
	$(CC) -c $(CFLAGS) $^ -o $@

stack.o: $(ALGORITHMS_PATH)/stack/stack.c
	$(CC) -c $(CFLAGS) $^ -o $@

queue.o: $(ALGORITHMS_PATH)/queue/queue.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Common library
lib.o: $(ALGORITHMS_PATH)/lib/lib.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) *.o
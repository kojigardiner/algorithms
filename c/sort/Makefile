CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..

objects := sort_client
all: $(objects)
	@echo Executing tests
	$(foreach obj, $(objects), ./$(obj);)

# Sort client
sort_client: lib.o sort_client.o sort.o
	@echo Compiling sort_client
	$(CC) $(CFLAGS) $^ -o $@

sort.o: $(ALGORITHMS_PATH)/sort/sort.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Common library
lib.o: $(ALGORITHMS_PATH)/lib/lib.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) *.o
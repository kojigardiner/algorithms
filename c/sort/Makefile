CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..

objects := sort_client kwic_client
all: $(objects)

# KWIC client
kwic_client: lib.o kwic_client.o sort.o
	@echo Compiling kwic_client
	$(CC) $(CFLAGS) $^ -o $@

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
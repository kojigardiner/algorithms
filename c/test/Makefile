CC = gcc
CFLAGS = -Wall -Werror -g
ALGORITHMS_PATH := ..
UNITY_PATH := ../../../Unity/src

objects := lib_test bag_test stack_test queue_test uf_test sort_test priority_queue_test
all: $(objects)
	@echo Executing tests
	$(foreach obj, $(objects), ./$(obj);)

# Priority Queue
priority_queue_test: lib.o priority_queue_test.o priority_queue.o unity.o
	@echo Compiling priority_queue_test
	$(CC) $(CFLAGS) $^ -o $@

priority_queue.o: $(ALGORITHMS_PATH)/priority_queue/priority_queue.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Sort
sort_test: lib.o sort_test.o sort.o unity.o
	@echo Compiling sort_test
	$(CC) $(CFLAGS) $^ -o $@

sort.o: $(ALGORITHMS_PATH)/sort/sort.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Union Find
uf_test: lib.o uf_test.o uf.o unity.o
	@echo Compiling uf_test
	$(CC) $(CFLAGS) $^ -o $@

uf.o: $(ALGORITHMS_PATH)/union_find/uf.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Bag
bag_test: lib.o bag_test.o bag.o unity.o
	@echo Compiling bag_test
	$(CC) $(CFLAGS) $^ -o $@

bag.o: $(ALGORITHMS_PATH)/bag/bag.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Stack
stack_test: lib.o stack_test.o stack.o unity.o
	@echo Compiling stack_test
	$(CC) $(CFLAGS) $^ -o $@

stack.o: $(ALGORITHMS_PATH)/stack/stack.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Queue
queue_test: lib.o queue_test.o queue.o unity.o
	@echo Compiling queue_test
	@$(CC) $(CFLAGS) $^ -o $@

queue.o: $(ALGORITHMS_PATH)/queue/queue.c
	@$(CC) -c $(CFLAGS) $^ -o $@

# Unity test framework
unity.o: $(UNITY_PATH)/unity.c
	@$(CC) -c $(CFLAGS) $^ -o $@

# Common library
lib_test: lib.o lib_test.o unity.o
	@$(CC) $(CFLAGS) $^ -o $@

lib.o: $(ALGORITHMS_PATH)/lib/lib.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) *.o
CC = gcc
CFLAGS = -Wall -Werror -g
PROJECTS_PATH := /Users/koji/Documents/Projects
ALGORITHMS_PATH := $(PROJECTS_PATH)/Algorithms_Princeton/c
UNITY_PATH := $(PROJECTS_PATH)/Unity/src

objects := bag_test stack_test queue_test uf_test sort_test
all: $(objects)
	@echo Executing tests
	$(foreach obj, $(objects), ./$(obj);)

sort_test: sort_test.o sort.o unity.o
	@echo Compiling sort_test
	@$(CC) $(CFLAGS) $^ -o $@

sort_test.o: sort_test.c
	@$(CC) -c $(CFLAGS) $^ -o $@

sort.o: $(ALGORITHMS_PATH)/sort/sort.c
	@$(CC) -c $(CFLAGS) $^ -o $@

uf_test: uf_test.o uf.o unity.o
	@echo Compiling uf_test
	@$(CC) $(CFLAGS) $^ -o $@

uf_test.o: uf_test.c
	@$(CC) -c $(CFLAGS) $^ -o $@

uf.o: $(ALGORITHMS_PATH)/union_find/uf.c
	@$(CC) -c $(CFLAGS) $^ -o $@

bag_test: bag_test.o bag.o unity.o
	@echo Compiling bag_test
	@$(CC) $(CFLAGS) $^ -o $@

bag_test.o: bag_test.c
	@$(CC) -c $(CFLAGS) $^ -o $@

bag.o: $(ALGORITHMS_PATH)/bag/bag.c
	@$(CC) -c $(CFLAGS) $^ -o $@

stack_test: stack_test.o stack.o unity.o
	@echo Compiling stack_test
	@$(CC) $(CFLAGS) $^ -o $@

stack_test.o: stack_test.c
	@$(CC) -c $(CFLAGS) $^ -o $@

stack.o: $(ALGORITHMS_PATH)/stack/stack.c
	@$(CC) -c $(CFLAGS) $^ -o $@

queue_test: queue_test.o queue.o unity.o
	@echo Compiling queue_test
	@$(CC) $(CFLAGS) $^ -o $@

queue_test.o: queue_test.c
	@$(CC) -c $(CFLAGS) $^ -o $@

queue.o: $(ALGORITHMS_PATH)/queue/queue.c
	@$(CC) -c $(CFLAGS) $^ -o $@

unity.o: $(UNITY_PATH)/unity.c
	@$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects) $(wildcard *.o)
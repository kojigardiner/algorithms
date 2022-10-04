CC = gcc
CFLAGS = -Wall -Werror -g
PROJECTS_PATH := /Users/koji/Documents/Projects
ALGORITHMS_PATH := $(PROJECTS_PATH)/Algorithms_Princeton/c
UNITY_PATH := $(PROJECTS_PATH)/Unity/src

objects := bag_test stack_test queue_test
all: $(objects)
	@echo Executing tests
	$(foreach obj, $(objects), ./$(obj);)

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
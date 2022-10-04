CC = gcc
CFLAGS = -Wall -Werror -g
PROJECTS_PATH := /Users/koji/Documents/Projects
ALGORITHMS_PATH := $(PROJECTS_PATH)/Algorithms_Princeton/c
UNITY_PATH := $(PROJECTS_PATH)/Unity/src
BAG_PATH := $(ALGORITHMS_PATH)/bag

objects := bag_test
all: $(objects)

bag_test: bag_test.o bag.o unity.o
	$(CC) $(CFLAGS) $^ -o $@

bag_test.o: bag_test.c
	$(CC) -c $(CFLAGS) $^ -o $@

unity.o: $(UNITY_PATH)/unity.c
	$(CC) -c $(CFLAGS) $^ -o $@

bag.o: $(ALGORITHMS_PATH)/bag/bag.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(objects)
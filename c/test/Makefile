CC = gcc
CFLAGS = -Wall -Werror -g -DUNITY_INCLUDE_DOUBLE
ALGORITHMS_PATH := ..
UNITY_PATH := ../../../Unity/src

objects := lib_test bag_test stack_test queue_test uf_test sort_test \
priority_queue_test symbol_table_test graph_test digraph_test paths_test \
cycle_test top_sort_test cc_test edge_test weighted_graph_test mst_test \
shortest_paths_test flow_network_test trie_test substring_search_test \
regex_test bit_io_test zip_test

all: $(objects)
	@echo Executing tests
	$(foreach obj, $(objects), ./$(obj);)

# Zip
zip_test: lib.o zip_test.o zip.o unity.o bit_io.o priority_queue.o
	@echo Compiling zip_test
	$(CC) $(CFLAGS) $^ -o $@

zip.o: $(ALGORITHMS_PATH)/zip/zip.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Bit IO
bit_io_test: lib.o bit_io_test.o bit_io.o unity.o
	@echo Compiling bit_io_test
	$(CC) $(CFLAGS) $^ -o $@

bit_io.o: $(ALGORITHMS_PATH)/bit_io/bit_io.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Regex
regex_test: lib.o regex_test.o regex.o unity.o stack.o graph.o paths.o bag.o queue.o
	@echo Compiling regex_test
	$(CC) $(CFLAGS) $^ -o $@

regex.o: $(ALGORITHMS_PATH)/regex/regex.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Substring search
substring_search_test: lib.o substring_search_test.o substring_search.o unity.o
	@echo Compiling substring_search_test
	$(CC) $(CFLAGS) $^ -o $@

substring_search.o: $(ALGORITHMS_PATH)/substring_search/substring_search.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Trie tests
trie_test: lib.o trie_test.o symbol_table.o unity.o stack.o queue.o
	@echo Compiling trie_test
	$(CC) $(CFLAGS) $^ -o $@

# Network flow tests
flow_network_test: lib.o flow_network_test.o flow_network.o flow_edge.o unity.o bag.o queue.o
	@echo Compiling flow_network_test
	$(CC) $(CFLAGS) $^ -o $@

flow_network.o: $(ALGORITHMS_PATH)/flow_network/flow_network.c
	$(CC) -c $(CFLAGS) $^ -o $@

flow_edge.o: $(ALGORITHMS_PATH)/flow_edge/flow_edge.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Shortest paths
shortest_paths_test: lib.o shortest_paths_test.o shortest_paths.o unity.o weighted_graph.o bag.o edge.o priority_queue.o stack.o
	@echo Compiling mst_test
	$(CC) $(CFLAGS) $^ -o $@

shortest_paths.o: $(ALGORITHMS_PATH)/shortest_paths/shortest_paths.c
	$(CC) -c $(CFLAGS) $^ -o $@

# MST
mst_test: lib.o mst_test.o mst.o unity.o weighted_graph.o bag.o edge.o priority_queue.o uf.o
	@echo Compiling mst_test
	$(CC) $(CFLAGS) $^ -o $@

mst.o: $(ALGORITHMS_PATH)/mst/mst.c
	$(CC) -c $(CFLAGS) $^ -o $@


# Weighted graph
weighted_graph_test: lib.o weighted_graph_test.o weighted_graph.o unity.o bag.o edge.o
	@echo Compiling weighted_graph_test
	$(CC) $(CFLAGS) $^ -o $@

weighted_graph.o: $(ALGORITHMS_PATH)/weighted_graph/weighted_graph.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Edge
edge_test: lib.o unity.o edge_test.o edge.o
	@echo Compiling cc_test
	$(CC) $(CFLAGS) $^ -o $@

edge.o: $(ALGORITHMS_PATH)/edge/edge.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Connected components
cc_test: lib.o cc_test.o graph.o unity.o bag.o cycle.o stack.o connected_components.o topological_sort.o
	@echo Compiling cc_test
	$(CC) $(CFLAGS) $^ -o $@

connected_components.o: $(ALGORITHMS_PATH)/connected_components/connected_components.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Cycle
top_sort_test: lib.o top_sort_test.o graph.o unity.o bag.o cycle.o stack.o topological_sort.o
	@echo Compiling top_sort_test
	$(CC) $(CFLAGS) $^ -o $@

topological_sort.o: $(ALGORITHMS_PATH)/topological_sort/topological_sort.c
	$(CC) -c $(CFLAGS) $^ -o $@


# Cycle
cycle_test: lib.o cycle_test.o graph.o unity.o bag.o cycle.o stack.o
	@echo Compiling cycle_test
	$(CC) $(CFLAGS) $^ -o $@

cycle.o: $(ALGORITHMS_PATH)/cycle/cycle.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Paths
paths_test: lib.o paths_test.o graph.o unity.o bag.o stack.o queue.o paths.o
	@echo Compiling paths_test
	$(CC) $(CFLAGS) $^ -o $@

paths.o: $(ALGORITHMS_PATH)/paths/paths.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Digraph
digraph_test: lib.o digraph_test.o graph.o unity.o bag.o
	@echo Compiling digraph_test
	$(CC) $(CFLAGS) $^ -o $@

# Graph
graph_test: lib.o graph_test.o graph.o unity.o bag.o
	@echo Compiling graph_test
	$(CC) $(CFLAGS) $^ -o $@

graph.o: $(ALGORITHMS_PATH)/graph/graph.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Symbol Table
symbol_table_test: lib.o symbol_table_test.o symbol_table.o unity.o stack.o queue.o
	@echo Compiling symbol_table_test
	$(CC) $(CFLAGS) $^ -o $@

symbol_table.o: $(ALGORITHMS_PATH)/symbol_table/symbol_table.c
	$(CC) -c $(CFLAGS) $^ -o $@

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
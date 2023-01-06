# Algorithms & Data Structures

This repository contains the following:
- C implementations of the algorithms and data structures detailed in the Algorithms 4th ed. textbook by Robert Sedgewick and Kevin Wayne. The textbook implementations are in Java, and I found porting them to C offered a chance to better understand their details.
- Test clients inspired by the textbook for some of the algorithms. Run `make` in each algorithm directory to build the test client(s).
- Unit test suites for each of the algorithms and data structures. These are written in the [Unity Test framework](https://github.com/ThrowTheSwitch/Unity). To run the tests, simply call `make` from the `tests` directory.
- My solutions to the Java programming assignments from the Coursera series based on the textbook ([Part I](https://www.coursera.org/learn/algorithms-part1), [Part II](https://www.coursera.org/learn/algorithms-part2)).

# Implementations
Most implementations below support storage of generic object types, including primitives or complex types (i.e. structs). Some complex types may require the client to implement comparator functions.

## Basic Data Structures

| Abstract Data Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Bag | Unordered collection | Add, Iteration | Linked list |
| Stack | LIFO collection | Push, Pop, Iteration | Linked list |
| Queue | FIFO collection | Enqueue, Dequeue, Iteration | Linked list |
| Priority Queue | Min- and Max-oriented sorted queue | Insert, Delete min/max | Binary heap |

## Symbol Tables
| Abstract Data Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Symbol Table | Key-value lookup | Put, Get, Contains, Iteration | Linked list, Binary search tree, Red-black binary search tree, Hash table (separate chaining), Hash table (linear probing) |
| Symbol Table (String) | String lookup | Put, Get, Contains, Iteration, Longest Prefix, Keys with Prefix, Keys that Match | R-way trie |

## Graphs
| Abstract Data Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Graph | Directed and undirected set of vertices and edges | | Array of adjacency lists (bags) |
| Weighted Graph | Directed and undirected set of vertices and weighted edges | | Array of adjacency lists (bags) |
| Paths | Find paths from source vertex to all reachable vertices | | Depth-first search, breadth-first search |
| Shortest Paths | Find paths of lowest weight in weighted graph from source vertex to all reachable vertices | | Dijkstra's algorithm
| Minimum Spanning Tree | Find tree of minimum weight in weighted graph that spans all vertices |  | Prim's algorithm, Kruskal's algorithm |
| Topological Sort | Compute precedence order for directed graph | | Depth-first search with reverse post-order via stack |
| Cycle | Determine if a graph has a cycle | | Depth-first search
| Connected Components | Find connected components for an undirected graph and strongly connected components for a directed graph | | Depth-first search for undirected, Kosaraju-Sharir algorithm for directed |

## Network Flow
| Abstract Data Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Flow Network | 

## Sorting
| Abstract Data Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Sort | Sort array of elements | Sort | Selectionsort, Insertionsort, Shellsort, Top-down mergesort, Bottom-up mergesort, Quicksort, Heapsort |
| Sort (String) | Sort array of strings | Sort | Least-significant-digit first, Most-significant-digit first, Three-way quicksort/radixsort |

## Strings
| Abstract Data Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Substring Search | Find first occurrence of a pattern in text | Search | Brute-force, Knuth-Morris-Pratt, Boyer-Moore, Rabin-Karp |
| Regular Expression |

## Other
| Abstract Data Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Symbol Graph | 
| Union Find | 
| Binary Search | 
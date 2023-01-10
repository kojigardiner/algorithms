# Algorithms & Data Structures

This repository contains the following:
- C implementations of the algorithms and data structures detailed in the *Algorithms 4th ed.* textbook by Robert Sedgewick and Kevin Wayne. The textbook implementations are in Java, and I found re-implementing them in C to help with my understanding.
- Test clients inspired by the textbook for some of the algorithms. Run `make` in each algorithm directory to build the test client(s).
- Unit test suites for each of the algorithms and data structures. These are written in the [Unity Test framework](https://github.com/ThrowTheSwitch/Unity). To run the tests, simply call `make` from the `tests` directory.
- My solutions to the Java programming assignments from the Coursera series based on the textbook ([Part I](https://www.coursera.org/learn/algorithms-part1), [Part II](https://www.coursera.org/learn/algorithms-part2)).

# Implementations
Most implementations below support storage of generic object types, including C primitives, C-strings, or structs. Custom structs require the client to implement comparator functions for some data structures.

For several of the data structures (e.g. symbol tables), multiple underlying implementation are available to be selected.

## Basic Data Structures

| Data Structure | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Bag | Unordered collection | Add | Linked list |
| Stack | LIFO collection | Push, Pop | Linked list |
| Queue | FIFO collection | Enqueue, Dequeue | Linked list |
| Priority Queue | Min- and Max-oriented sorted queue | Insert, Front, Delete Min/Max | Binary heap |

## Sorting
| Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Generic Sort | Sort array of items | Sort | Selectionsort, Insertionsort, Shellsort, Top-down mergesort, Bottom-up mergesort, Quicksort, Heapsort |
| String Sort | Sort array of strings | Sort | Least-significant-digit (MSD) first, Most-significant-digit (MSD) first, Three-way quicksort/radixsort |

## Symbol Tables (Maps)
| Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Generic Symbol Table | Key-value lookup | Put, Get, Contains | Linked list, Binary search tree, Red-black binary search tree, Hash table (separate chaining), Hash table (linear probing) |
| String Symbol Table | String lookup | Put, Get, Contains, Longest Prefix, Keys with Prefix, Keys that Match | R-way trie |

## Graphs
| Data Structure | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Graph / Weighted Graph | Set of vertices contained by directed or undirected edges | Add Edge, Iterate over Adjacent Edges | Array of adjacency lists (bags) |
| Paths | Find paths in a graph from source vertex to all reachable vertices | Has Path To, Iterate over Vertices | Depth-first search, Breadth-first search |
| Shortest Paths | Find paths of lowest weight in a weighted graph from source vertex to all reachable vertices | Has Path To, Distance To, Iterate over Vertices | Dijkstra's algorithm
| Minimum Spanning Tree | Find tree of minimum weight in weighted graph that spans all vertices | Iterate over Edges | Prim's algorithm, Kruskal's algorithm |
| Topological Sort | Compute precedence order for directed graph | Iterate over Vertices | Depth-first search with reverse post-order via stack |
| Cycle | Determine if a graph has a cycle | Has Cycle, Iterate over Vertices| Depth-first search
| Connected Components | Find connected components for an undirected graph and strongly connected components for a directed graph | Count, ID for Vertex, Are Vertices Connected | Depth-first search for undirected, Kosaraju-Sharir algorithm for directed |
| Flow Network | Find max flow through a network where edges support a max flow capacity | Maxflow, Incut | Ford-Fulkerson algorithm, Breadth-first search

## String Search
| Type | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Substring Search | Find first occurrence of specific string in text | Search | Brute-force, Knuth-Morris-Pratt, Boyer-Moore, Rabin-Karp |
| Regular Expression Search | Find first occurrence of pattern (including metacharacters) in text | Recognizes | Non-deterministic finite automata (NFA) as a graph

## Data Compression
| Data Structure | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Bit I/O | Interface for reading/writing files with bit-level granularity | Read/Write Bit, Bits, Byte, Int | Wrapper for fread/fwrite | 
| Zip | Interface for compressing and expanding arbitrary files | Compress, Expand | Run-Length Encoding, Huffman Compression, LZW Compression |

## Other
| Data Structure | Description | Primary Operations | Underlying Implementation(s) |
| :------: | :------: | :------: | :------: |
| Union Find | Solve the dynamic connectivity problem | Union, Connected | Quick-find, Quick-union with path compression
| Binary Search | Find index of a integer in a sorted array of integers | Index Of | Binary search

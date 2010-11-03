# Fibonacci Heap

This is a simple fibonacci heap, supporting the standard operations:

* Insert
* Merge
* Extract Minimum
* Decrease Key

We also have a non-standard *find* function; this is only for testing and should not be used in production as finding in a heap is O(n).

Fibonacci heaps are slow and have significant storage overheads (4 pointers per node, plus an int and a bool for housekeeping.) They have a better complexity than binomial heaps only when you are doing significantly more merges or decrease key operations than extract minimum operations.

This implementation should show how Fibonacci heaps work; it is not intended to be highly performant.

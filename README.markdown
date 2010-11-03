# Fibonacci Heap

This is a simple fibonacci heap, supporting the standard operations:

* Insert
* Merge
* Extract Minimum
* Decrease Key

We also have a non-standard *find* function; this is only for testing and should not be used in production as finding in a heap is O(n).

Fibonacci heaps are slow and have significant storage overheads (4 pointers per node, plus an int and a bool for housekeeping.) They have a better complexity than binomial heaps only when you are doing significantly more merges or decrease key operations than extract minimum operations.

This implementation should show how Fibonacci heaps work; it is not intended to be highly performant.

## Sample output
The test program should produce a heap that looks like this:

![A diagram of a heap produced by this program](https://github.com/robinmessage/fibonacci/raw/master/sample.png "Sample Fibonacci Heap")

Half-headed arrows are used for next and previous pointers. Full arrows are used for child and parent pointers. Filled arrowheads are used for next and child pointers; white arrowheads are for previous and parent pointers. Note that we synthesis child pointers to all the children of a node; in actuality, only one of these pointers is stored, since we can find sibling nodes. Marked nodes are grey and hoepfully the minimum is obvious.

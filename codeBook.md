Data from sorting methods
=========================

# The data produced by sample

The program sample performs several sorting methods with diverse
parameter and measures their executions times on sequences stored in
single linked list.

Sequences are ramdoly generated.

Performing

	make

should build sample and other programs.

Type
	
	./sample --help

in order to know the program interface

## The row structure

It is

	n, sort-method-tag, shuffle-factor insertion-threshold, execution-time

- n is the number of items stored in the single list

- sort-method-tag is a label for identifying the sorting method

- shuffle-factor is the factor of shuffling. There are two ways for
  generating data. The first one is to generate a sorted sequence of n
  items,. then shuffle-factor*n inversions are done. In this way the
  sequence is shuffled accordind to the factor. If shuffle-factor is 0,
  then the the sequence remains sorted. If shuffle-factor is 1, then the
  sequence is randomly generated, without swaps,

- insertion-threshold the programmed sorting methods call to the
  insertion sort for sequences shorter of insertion-threshold

- execution-time the duration of sorting method in mseconds
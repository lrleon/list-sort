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


# The sorting methods

The idea of this study is to determine which is the best general method
for sorting single linked list.  We considere and combine them three
methods: quicksort, mergesort and insertionsort.

Insertion sort is called when the list size is less than the value of
insertion_threshold global variable. The reason for that is that the
insertion sort tends to be faster than mergesort and quicksort for small
sizes. So, one first objetive is know this threshold value: that is, what
is the larger size of a single linked list for which the insertionsort
is faster than quicksort and mergesort?

In this study we test the following methods (the tag and factor name are
between ()):

1-. (insertionsort, Insertion): the classical insertion sort.
2-. (quicksort_w, Quicksort): pure quicksort.
3-. (mergesort_w, Mergesort): pure mergesort.
4-. (merge_insertsort, Mergesort.Insertion): mergesort combined with
insertion for partition sizes lesser that insertion_threshold.
4-. (quicksort_insertion, Quicksort.Insertion): quicksort combined with
insertion for partition sizes lesser that insertion_threshold.
5-.
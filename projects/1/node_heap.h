//
// file: node_heap.h
// author: ben k steele, bks@cs.rit.edu
//
//# The heap is a minimum-ordered heap, and 
//# its implementation is specific to the variable-length coding application.

#ifndef NODE_HEAP_H
#define NODE_HEAP_H

#include <stdio.h>
#include <stdlib.h>

/// NUL is the 0, or null character, value in the ASCII code representation.
#define NUL   '\0'

/// NUM_BITS is the number of bits in the extended ASCII code representation.
#define NUM_BITS   8

/// NSYMS is the maximum number of distinct symbols in the input text.
#define NSYMS      1 << NUM_BITS

/// MAX_CODE is the worst case code length
// while this is correct, it breaks the old test_heap #define MAX_CODE  NSYMS
#define MAX_CODE  30
//#define MAX_CODE  NSYMS

/// The Symbol structure stores:
/// <ul><li>the <code>symbol</code> (a character),
/// <li>its <code>codeword</code> representation, 
/// <li>the next usable <code>bit</code> in (aka length of)
/// the <code>codeword</code> representation, and
/// <li>its <code>frequency</code> of occurrence in the source.</ul>
/// <p>Entries in a Symbol's codeword string with (index >= bit) are unused.
/// Each character in a codeword that is <b>unused</b>
/// must be set/initialized to indicate that it is unused and NUL.
/// <p>Each Symbol instance needs initialization before it can be used.
///
typedef struct Symbol_S {
    /// frequency is the frequency of this symbol.
    size_t frequency;

    /// index of next bit in this symbol's codeword. range [0...MAX_CODE-1].
    size_t bit;

    /// symbol stores the character symbol being encoded (e.g. 'A').
    unsigned char symbol;

    /// codeword is the variable length encoding string (e.g. "1011").
    char codeword[MAX_CODE];
} Symbol;

/// The Node structure stores:
/// <ul><li> <code>frequency</code>, the cumulative frequency of occurrence
/// of symbols in the source text,
/// <li><code>num_valid</code>, the count of how many symbols are valid,
/// and <li><code>syms</code>, the collection (an array) of symbols.</ul>
/// <p>Entries in a Node's syms array with (index > num_valid) are unused.
/// Any Symbol in a Node structure that is <b>unused</b>
/// must be set/initialized to indicate that it is unused and empty.
///
typedef struct Node_S {
    /// cumulative frequency of the symbol objects inside this Node.
    size_t frequency;

    /// count of number of valid symbols in syms or index of next to add.
    size_t num_valid;

    /// syms is an array whose capacity is the number of distinct symbols.
    Symbol syms[NSYMS];

} Node;

/// The Heap struct holds a fixed-capacity array of Node structures
/// whose ordering value field is called 'frequency' (see struct Node_S).
/// <p>The heap structure stores:
/// <ul><li> <code>capacity</code>, the initialized maximum capacity,
/// <li><code>size</code>, the current number of valid entries in the heap,
/// and <li><code>array</code>, the Node structures.</ul>
/// <p>Entries in a heap's array between size and index capacity-1 are unused.
/// Any entry in a Heap structure that is <b>unused</b>
/// must be set/initialized to indicate that it is unused and empty.
///
typedef struct Heap_S {
    /// capacity is the capacity of the heap's array.
    size_t capacity;

    /// size is the current number of valid entries in the heap array.
    size_t size;

    /// array holds Node structures.
    Node array[NSYMS];
} Heap;

// // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // public Symbol, Node and Heap related functions
// // // // // // // // // // // // // // // // // // // // // // // //

/// read_symbols reads characters from standard input, calculates
/// each symbol's frequency of appearance, and counts the number of
/// distinct symbols it sees.
/// @pre: maxcount is a the capacity of the syms array.
/// @post: syms array is initialized and filled with histogram information.
/// @post: syms is ordered so that all used entries are before unused.
/// @post: unused syms array entries are initialized as unused.
///
int read_symbols( size_t maxcount, Symbol syms[] ) ;

/// heap_init initializes the heap storage with all unused Node entries.
/// heap is a <em>pointer</em>, a reference to a heap structure.
/// @pre: heap is a valid pointer and heap->capacity is uninitialized.
/// @post: heap->array has been initialized with unused Node entries.
/// @post: heap->size == 0.
/// @post: heap->capacity is initialized to the capacity of heap->array.
///
void heap_init( Heap * heap );

/// heap_make fills heap with symbols from symlist and <em>heapifies</em> it.
/// heap is a <em>pointer</em>, a reference to an initialized heap.
/// length is the length of the symlist.
/// <p>algorithm:
/// Take each symbol from symlist and put it into the
/// next note in the heap, initializing the node's frequency
/// to that of the symbol, and setting the node's num_valid to 1.
/// @pre: entries in symlist[0, ..., length-1] are valid Symbol objects.
/// @pre: heap is an initialized, unused heap of capacity >= length.
/// @pre: length <= heap->capacity. symlist is array of Symbol structs.
/// @post: heap->array is filled with Node entries in min-heap order.
/// @post: heap->size == length.
/// @post: heap->array[i].syms[0] is valid symbol for i in [0, ..., length-1].
///
void heap_make( Heap * heap, size_t length, Symbol symlist[] );

/// heap_add adds one more node to the current heap.
/// heap_add replaces what is expected to be an unused Node entry.
/// heap is a <em>pointer</em>, a reference to an initialized heap.
/// @pre: heap->size < heap->capacity (fatal error to exceed heap capacity)
/// @post: heap->size has increased by 1.
/// @post: heap->array is in heap order.
/// <p>algorithm:
/// Since the heap->array is fixed capacity, heap_add cannot enlarge it;
/// instead the add overwrites an unused node in the heap structure.
/// The add puts the node's values into the next location in the heap's array.
/// <p>
/// After assigning the values, heap_add restores heap order
/// by sifting the last entry up the heap from the last location.
/// Finally it increments the size of the heap.
/// <p>
/// If there is no room for more nodes, then it is a fatal error.
/// However, the array is sized to hold enough nodes for all the symbols
/// it can encounter. (Running out of nodes would indicate a programming
/// design error.)
///
void heap_add( Heap * heap, Node node );

/// heap_remove removes and returns a node structure.
/// heap is a <em>pointer</em>, a reference to an initialized heap.
/// @pre: heap->size > 0 (fatal error to remove from an empty heap)
/// @post: heap->size has decreased by 1.
/// @post: remaining heap is in proper heap order.
/// <p>algorithm:
/// Save the top of the heap at index 0.
/// Reduce the size of the heap by
/// copying the last heap entry into the top location and
/// replacing the last location's entry with values representing an
/// unused Node object.
/// Restore the heap order by sifting the top entry down the heap.
/// Return the saved top entry.
///
Node heap_remove( Heap * heap );

#endif // NODE_HEAP_H

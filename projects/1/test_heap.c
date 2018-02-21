// 
// File: test_heap.c 
// 
// test_heap.c tests the node_heap module using random numbers for
// frequency values in the Node structures added/removed from the node heap.
// 
// @author bks: ben k steele
// 
// // // // // // // // // // // // // // // // // // // // // // // // 

// srandom requires defining _BSD_SOURCE to bring in correct typedefs.
#define _BSD_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "node_heap.h"

//# returns the index of the parent of location i.
#define Parent_Index( i ) ( i - 1 ) / 2

//# returns the index of the left child of location i.
#define Left_Child_Index( i )     ( i * 2 ) + 1

//# returns the index of the right child of location i.
#define Right_Child_Index( i )    ( i * 2 ) + 2

/// generateRandoms fills values with randoms between 0 and 8 times the count.

void generateRandoms( size_t count, size_t values[] ) {

    size_t max = count << 3;
    for ( size_t i = 0; i < count; ++i ) {
        values[i] = (size_t)(random() % max );
    }
}

/// addRandom adds count Nodes containing given frequencies

void addRandom( Heap * heap, size_t count, size_t freqs[] ) {

    assert( heap );
    assert( count < heap->capacity );

    Node node ;

    for ( size_t i = 0; i < count; ++i ) {

        node.frequency = freqs[i];  // all the heap cares about!
        heap_add( heap, node );
    }
}

/// Display the heap as a tree with each child value indented
/// from its parent value. Traverse the heap tree in preorder.

void displayHeap( Heap * heap, size_t startI, int indent) {

    if ( startI < heap->size ) {

        printf( "%*c %zu\n", indent, ' ', heap->array[startI].frequency );

        displayHeap( heap, Left_Child_Index( startI ), indent + 4 );
        displayHeap( heap, Right_Child_Index( startI ), indent + 4 );
    }
}

static Heap test_heap;

/// testHeap1 tests filling and emptying a node Heap.
/// Test creates a heap, heap_adds random test data, displays it,
/// then heap_removes all the data and displays again (empty).
///
void testHeap1( size_t count ) {

    // Tester does not test heap_make, which manipulates Symbols inside Node.
    // Instead the test uses heap_add and heap_remove, which only process
    // the Node fields.
    // Making the heap's Node instances hold symbols correctly is left
    // as part of the assignment exercise.

    printf( "begin testHeap1( %zu ):=============================\n", count );

//Heap test_heap;
    heap_init( &test_heap );

    size_t values[count];

    generateRandoms( count, values );

    printf( "adding... " );
    addRandom( &test_heap, count, values );

    printf( "Heap size is now %zu\n", test_heap.size );
    printf( "displayHeap...\n" );
    displayHeap( &test_heap, 0, 0 );

    Node node;

    printf( "Removing node frequencies in order... " );
    size_t n = test_heap.size;
    for ( size_t i = 0; i < n; ++i ) {

        node = heap_remove( &test_heap );
        if ( i % 10 == 0 ) printf( "\n" );
        printf( "%zu ", node.frequency );
    }
    printf( "\n" );
    printf( "\nHeap size is now %zu\n", test_heap.size );
    printf( "displayHeap...\n" );
    displayHeap( &test_heap, 0, 0 );
    printf( "\nend testHeap1()\n" );
}

/// testHeap2 tests filling and partly emptying a node Heap.
///
void testHeap2( size_t count ) {

    printf( "begin testHeap2( %zu ):=============================\n", count );

    //Heap test_heap;
    heap_init( &test_heap );

    size_t values[count];

    generateRandoms( count, values );

    printf( "adding... " );
    addRandom( &test_heap, count, values );

    printf( "Heap size is now %zu\n", test_heap.size );
    printf( "displayHeap...\n" );
    displayHeap( &test_heap, 0, 0 );

    Node node;

    size_t n = (size_t)(random() % ( count ) );

    printf( "Removing %zu nodes ... ", n );
    for ( size_t i = 0; i < n; ++i ) {

        node = heap_remove( &test_heap );
        if ( i % 10 == 0 ) printf( "\n" );
        printf( "%zu ", node.frequency );
    }
    printf( "\n" );
    printf( "\nHeap size is %zu. ", test_heap.size );
    printf( " and should be %zu\n", count - n );

    printf( "added 2 more entries, Big and Small.\n" );
    node.frequency = 8888;
    heap_add( &test_heap, node );
    node.frequency = 0;  // smallest value for size_t (unsigned)
    heap_add( &test_heap, node );
    printf( "displayHeap...\n" );
    displayHeap( &test_heap, 0, 0 );

    node = heap_remove( &test_heap );
    printf( "\nRemoved entry frequency is %zu. ", node.frequency );
    printf( " and should be %d\n", 0 );
    printf( "\nend testHeap2()\n" );
}

/// main function runs a test suite on the node_heap module implementation.
/// @returns errorCode  the error Code; 0 if no error

int main( void ) {

    srandom( 63 ); // seed the generator
    int cases[] = { 1, 2, 3, 5, 11, 12, 23 };
    for ( size_t j = 0; j < sizeof( cases ) / sizeof( int ); ++j ) {

        testHeap1( cases[j] );
        testHeap2( cases[j] );
    }
    return 0 ;
}

// // // // // // // // // // // // // // // // // // // // // // // // 
// 

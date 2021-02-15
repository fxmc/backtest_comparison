/*
	Provides interface to dynamic arrays in C
	At present no bells and whistles.  Primarily focused on
	providing a container for the csv laoder

	In terms of dealing with errors: at present there is no graceful way.  if any error is
	detected in the code, via the errorhandling in C, and exit is executed.

	This leads to a certain conundrum: freein up memory.  The free will always act upon the 
	array that is just being manipulated and upon which an operation failed.  However,
	if we have many more things allocated in the meantime: oops.  

	Here we are relying upon the operating system to free up all memory.  One possible solution is
	to keep track of all allocated pointers and free them ourselves, or have the signal.h library
	take care of that.
*/

#pragma once
#include <string.h>
#include <stdbool.h>
#include "../conv.h"

/*
	Main structure for dealing with variable arrays. This structure contains several useful things:
	The length, which is the number of elements stored in the array.  The allocsize which is the
	actual amount of memory allocated.  

	If more elements are inserted into the array than current memory is available, new memory is obtained.
*/
typedef struct {
	size_t length;
	size_t allocsize;
	csvloadtype array[];
} arraytype;

/*
	allocate_array allocates enough space for an array of type csvloadtype[] of length arraylength.
	It returns a pointer to the structure arraytype.
	If an error occurs in the allocation the program is exited.
*/
arraytype* allocate_array(size_t arraylength);

/*
	free_array frees up the allocated memory obtained by allocate_array
	The input parameter is a pointer to the arraytype structure which was created with the allocation function
*/
void free_array(arraytype* array);

/*
	push_element is the function to be used to insert new elements into the array.  If array extension is
	permitted then when the array bound is hit new memory will be allocated to extend the array

	The inputs are:
		arraytype** array : a pointer to a pointer.  This is necessary due to the fact that the realloc 
		                    function disposes of the old pointer [invalidates it], so we need to return
						    by reference the new pointer created by realloc

							When calling this function we usually define a pointer to the structure and then
							pass the address to this pointer:

							arraytype* pa;
							push_element(&pa, ....);

		csvloadtype elt   : the element to be pushed at the end of the array
		bool extend       : can be true or false.  The state indicates if the array should be ectended or not
		                    in case of buffer overflow.  If set to false and a buffer overflow occurs, the
							pogram exits.
*/
void push_element(arraytype** array, csvloadtype elt, bool extend);

/*
	set_element function sets an element in the array.  If the index supplied is out of bounds the program
	exits.

	The inputs are:
		arraytype* array  : pointer to the array structure
		size_t index      : index where the element should be set.  Note we are indexing base 0
		csvloadtype elt   : the element to be stored in the array
*/
void set_element(arraytype* array, size_t index, csvloadtype elt);

/*
	get_element function obtains the element in the array at a particular index.  If the index supplied is out of
	bounds the program exits.

	The inputs are:
		arraytype * array : pointer to the array structure
		size_t index      : index to the element being extracted out of the array
*/
csvloadtype get_element(arraytype* array, size_t index);

/*
	Gets the length of the array
*/
size_t get_length(arraytype* array);


/*
	Gets the allocated memory size of the array
*/
size_t get_alloc_size(arraytype* array);


/*
	Deepcopy of the original array. This means that a new array is allocated and the values copied over.  I.e. copy by value not
	by reference
*/
arraytype* deep_copy_array(arraytype* array);


/*
	Fills the array first n values of the array with a specific value.  Array needs to be pre-allocated
*/
void fill_array(arraytype* array, csvloadtype fill_value, size_t n);


/*
	Perform an element-wise summation of the two arrays and return the result
*/
arraytype* add_array(arraytype* a, arraytype* b);


/*
	Perform an elemenet-wise product of two arrays [i.e. dot product of two vectors] and return result.
*/
arraytype* mmul_array(arraytype* a, arraytype* b);
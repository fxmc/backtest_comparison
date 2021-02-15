/*
	Library for interfacing to double[] arrays in a dynamic fashion
	The idea is that we can then use these to walk through them.

	The key here is the push_element function which dynamically allocates more 
	memory.  [Rather than linear in an exponential way].

	The struct used to keep all these together is:

	typedef {
		allocsize,
		length,
		double array[];
	} arraytype

	Note a slight trick here in the push_element function: the arraypointer is passed as a pointer to a pointer so that
	ultimately we can use the new result from realloc.

*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "arrays.h"


/*
	Function
*/
arraytype* allocate_array(size_t arraylength) {
	arraytype* a = (arraytype*)malloc(sizeof(arraytype) + sizeof(csvloadtype) * arraylength);

	if (a == NULL) {
		printf("Error in allocating array: %d\n", errno);
		printf("Error message: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	(*a).allocsize = arraylength;
	(*a).length = 0;
	
	return a;
}


void free_array(arraytype* array) {
	free(array);
}


void push_element(arraytype** array, csvloadtype elt, bool extend) {
	if ((**array).length < (**array).allocsize) {
		(**array).array[(**array).length] = elt;
		(**array).length++;
	}
	else if (extend) {
		size_t new_alloc_size = 2 * (**array).allocsize;
		size_t new_size = sizeof(arraytype) + sizeof(csvloadtype) * new_alloc_size;
		arraytype* new_array = (arraytype*)realloc(*array, new_size);
		if (new_array == NULL) {
			free(array);
			printf("Error!  Could not extend array!\n");
			exit(EXIT_FAILURE);
		}
		*array = new_array;
		(**array).allocsize = new_alloc_size;
		push_element(array, elt, extend);
	}
	else {
		printf("Out of bounds error in pushing element!  Length: %zu and allocation: %zu\n", (**array).length+1, (**array).allocsize);
		exit(EXIT_FAILURE);
	}
}


void set_element(arraytype* array, size_t index, csvloadtype elt) {
	if ( (index < 0) || (index >= (*array).length)) {
		printf("Out of bounds error in setting element! Length: %zu trying to set at position: %zu, index: %zu\n", (*array).length, index + 1, index);
		exit(EXIT_FAILURE);
	}
	array->array[index] = elt;
}


csvloadtype get_element(arraytype* array, size_t index) {
	if ((index < 0) || (index >= (*array).length)) {
		printf("Out of bounds error in getting element! Length: %zu trying to get at position: %zu, index: %zu\n", (*array).length, index + 1, index);
		exit(EXIT_FAILURE);
	}
	return array->array[index];
}


size_t get_length(arraytype* array) {
	return array->length;
}


size_t get_alloc_size(arraytype* array) {
	return array->allocsize;
}


arraytype* deep_copy_array(arraytype* array) {
	arraytype* result = allocate_array(array->length);
	for (size_t i = 0; i < array->length; ++i) {
		push_element(&result, array->array[i], false);
	}
	return result;
}


void fill_array(arraytype* array, csvloadtype fill_value, size_t n) {
	if (fill_value == 0.0) {
		memset(array->array, 0, n * sizeof(csvloadtype));  // spped trick
		array->length = n;
	}
	else {
		for (size_t i = 0; i < n; i++) {
			push_element(&array, fill_value, false);
		}
	}

}


arraytype* add_array(arraytype* a, arraytype* b) {
	assert(a->length == b->length);
	arraytype* result = allocate_array(a->length);
	for (size_t i = 0; i < a->length; i++)
		push_element(&result, a->array[i] + b->array[i], false);
	return result;
}


arraytype* mmul_array(arraytype* a, arraytype* b) {
	assert(a->length == b->length);
	arraytype* result = allocate_array(a->length);
	for (size_t i = 0; i < a->length; i++)
		push_element(&result, a->array[i] * b->array[i], false);
	return result;
}

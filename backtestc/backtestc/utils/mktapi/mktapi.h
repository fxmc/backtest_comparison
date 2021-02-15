/*
	This is really a CSV loader niched down to the YAHOO finance csv output

*/
# pragma once

#include "../conv.h"
#include "../arrays/arrays.h"

/*
	Structure which holds the OHLC of the csv file.
*/
typedef struct {
	csvloadtype open, high, low, close;
} OHLC;

/*
	Function to load the data from FILENAME.  The inputs are pointers to doubles.  The result
	of the function specifies the number of elements in each array.  The function guarantees
	that all the arrays have the same length.

	We will assume that the arrays provided have already been allocated.  
	If the load exceeds the array allocation the arrays will be reallocated as required.
*/
size_t loadcsv(char* FILENAME, arraytype** open, arraytype** high, arraytype** low, arraytype** close, int skiprows);
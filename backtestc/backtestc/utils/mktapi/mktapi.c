/*
Library to help load the yahoo csv files in the ../data folder.
The format is:
	Line 1: Header: date,open,high,low,close,volume,adjclose
	Line 2 onwards: m/d/yyyy,<float>,<float>,<float>,<float>,<long>,<float>
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../conv.h"
#include "../mktapi/mktapi.h"
#include "../arrays/arrays.h"
#include "../debugh/debug.h"


/*
	Local function for opening hte file handle and error checking that the open procedure was successful
*/
static FILE* open_file(const char filename[]) {
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error trying to load data file:\n");
		printf("Value of error number: %d\n", errno);
		printf("Error message: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fp;
}

/*
	Simple local parser to get the OHLC from a line of the csv file.  
		buf is the input comma-separated string
		OHLC is the structure containing the open / high / low / close
*/
static void get_ohlc(char *buf, OHLC* ohlc) {
	char* field = NULL;
	int fieldnumber = 0;

	field = strtok(buf, ",");
	fieldnumber = 0;
	while (field != NULL) {
		size_t len = strlen(field);
		if (len && field[len - 1] == '\n') {
			field[len - 1] = '\0';
		}
		switch (fieldnumber) {
		case 0:
		case 5:
		case 6:
			break;
		case 1: 
			(*ohlc).open = atof(field);
			break;
		case 2: 
			(*ohlc).high = atof(field);
			break;
		case 3: 
			(*ohlc).low = atof(field);
			break;
		case 4: 
			(*ohlc).close = atof(field);
			break;
		default:
			printf("Error encountered in reading the row of the input file!\n");
			exit(EXIT_FAILURE);
			break;
		}
		fieldnumber++;
		field = strtok(NULL, ",");
	}
}

/*
	CSV loader for Yahoo Finance formatted csv files.
	The location of the csv file is specified by the file-local FILENAME constant
	The dynamic arrays are expected to be allocated already!
	We are using pointers to pointers here so that we can actually re-allocate if necessary.
*/
size_t loadcsv(char* FILENAME, arraytype** open, arraytype** high, arraytype** low, arraytype** close, int skiprows) {
	FILE* fp = open_file(FILENAME);
	char buf[LINELENGTH];  // a line of the csv
	int rownumber = 0;
	OHLC ohlc;

	while (fgets(buf, LINELENGTH, fp)) {
		if (rownumber < skiprows) { rownumber++; continue;}
	
		get_ohlc(buf, &ohlc);

		push_element(open, ohlc.open, true);
		push_element(high, ohlc.high, true);
		push_element(low, ohlc.low, true);
		push_element(close, ohlc.close, true);
		
		++rownumber;
	}
	fclose(fp);

	if (get_length(*open) == get_length(*high) &&
		get_length(*open) == get_length(*low) &&
		get_length(*open) == get_length(*close))
		return get_length(*open);

	printf("Failed to load the csv file correctly.  The OHLC arrays have different lengths!");
	exit(EXIT_FAILURE);
}

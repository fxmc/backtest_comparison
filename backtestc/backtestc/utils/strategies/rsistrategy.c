/*
	RSI Strategy.
*/
#include <stdbool.h>
#include <math.h>
#include "../arrays/arrays.h"


/*
	Larry Connors' RSI Strategy.  
*/
void run_rsi_strategy(arraytype* close, arraytype* ma_long, arraytype* ma_short, arraytype* rsi_n, 
	double rsi_lower, size_t start_index, arraytype* posn, arraytype* cash) {
	double shares;
	bool long_posn = false;

	for (size_t idx = start_index; idx < close->length; ++idx) {
		posn->array[idx] = posn->array[idx - 1];
		cash->array[idx] = cash->array[idx - 1];

		if (long_posn) {
			if (close->array[idx] > ma_short->array[idx]) {
				long_posn = false;
				shares = posn->array[idx];
				posn->array[idx] = 0.0;
				cash->array[idx] = cash->array[idx] + shares * close->array[idx];
			}
		}

		if (!long_posn) {
			if ((close->array[idx] > ma_long->array[idx]) && (rsi_n->array[idx] < rsi_lower)) {
				long_posn = true;
				shares = floor(cash->array[idx] / close->array[idx]);
				posn->array[idx] = shares;
				cash->array[idx] = cash->array[idx] - shares * close->array[idx];
			}
		}
	}

}
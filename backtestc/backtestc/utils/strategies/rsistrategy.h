#pragma once

#include "../arrays/arrays.h"

/*
	Larry Connors' RSI Strategy.  Takes in the various pre-calculated inputs:
		1) Close
		2) Long MA
		3) Short MA
		4) RSI(2)
	
	as well as the various parameters:
		1) Lower RSI value, e.g. buy if RSI(2) < 20.
*/
void run_rsi_strategy(arraytype* close, arraytype* ma_long, arraytype* ma_short, arraytype* rsi_n, double rsi_lower, size_t posn_start, arraytype* posn, arraytype* cash);
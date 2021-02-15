#pragma once

#include <stdbool.h>
#include "../arrays/arrays.h"

/*
	Takes an array and generates the n-step difference.  The suffix _at indicates that it operates on our bespoke arraytype type, and not
	on double[] arrays.
	Specifically: a_d[i] = a[i] - a[i-n]
	The values a[0], a[1], ... a[n-1] are set to NAN;
*/
arraytype* diff_at(arraytype* array, size_t diffstep); 

/*
	Calucates the mean of the values in the arraytype between st_idx inclusive and ed_idx exclusive [Python convention].
	If ignore_nan is set to True NAN values will be ignored.
*/
double mean_at(arraytype* array, size_t st_idx, size_t ed_idx, bool ignore_nan);

/*
	Calucates the variance of the values in the arraytype between st_idx inclusive and ed_idx exclusive [Python convention].
	If ignore_nan is set to True NAN values will be ignored.
*/
double var_at(arraytype* array, size_t st_idx, size_t ed_idx, bool ignore_nan);

/*
	Calucates the standard deviation of the values in the arraytype between st_idx inclusive and ed_idx exclusive [Python convention].
	If ignore_nan is set to True NAN values will be ignored.
*/
double std_at(arraytype* array, size_t st_idx, size_t ed_idx, bool ignore_nan);


/*
	Calucates the moving average of the arraytype array for the period specified.
*/
arraytype* ma_at(arraytype* array, size_t period);


/*
	Calucates the RSI of the arraytype array for the period specified.
*/
arraytype* rsi_at(arraytype* array, size_t period);

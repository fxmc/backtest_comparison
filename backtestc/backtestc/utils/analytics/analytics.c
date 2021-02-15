/*
	Note that the convetions here on end index is like in python.  It points one beyond.
*/

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../arrays/arrays.h"
#include "../conv.h"


static void diff_d(csvloadtype array[], size_t length, size_t diffstep) {
	size_t i;

	for (i = length - 1; i >= diffstep; --i) {
		array[i] = array[i] - array[i - 1];
	}

	for (i = 0; i < diffstep; ++i) {
		array[i] = (csvloadtype)NAN;
	}
}


arraytype* diff_at(arraytype* array, size_t diffstep) {
	arraytype* result = deep_copy_array(array);   // program will crash here if unsuccesful
	diff_d(result->array, result->length, diffstep);
	return result;
}


static double mean_d(csvloadtype array[], size_t st_idx, size_t ed_idx) {
	csvloadtype sum = 0.0;
	for (size_t i = st_idx; i < ed_idx; ++i) {
		sum += array[i];
	}
	return sum / (ed_idx - st_idx);
}


static double mean_d_ignore_nan(csvloadtype array[], size_t st_idx, size_t ed_idx) {
	csvloadtype sum = 0.0;
	size_t ignored_nan = 0;
	for (size_t i = st_idx; i < ed_idx; ++i) {
		if (isnan(array[i])) {
			++ignored_nan;
			continue;
		}
		sum += array[i];
	}
	return sum / (ed_idx - st_idx - ignored_nan);
}


double mean_at(arraytype* array, size_t st_idx, size_t ed_idx, bool ignore_nan) {
	if (ignore_nan)
		return mean_d_ignore_nan(array->array, st_idx, ed_idx);
	else
		return mean_d(array->array, st_idx, ed_idx);
}


static double var_d(csvloadtype array[], size_t st_idx, size_t ed_idx) {
	csvloadtype m = array[st_idx];
	csvloadtype s = 0.0;
	csvloadtype delta = 0.0;
	for (size_t i = st_idx + 1; i < ed_idx; i++) {
		delta = array[i] - m;
		m = m + delta / (i - st_idx + 1);
		s = s + delta * (array[i] - m);
	}
	return s / (ed_idx - st_idx - 1);
}


static double var_d_ignore_nan(csvloadtype array[], size_t st_idx, size_t ed_idx) {
	while (isnan(array[st_idx])) ++st_idx;
	csvloadtype m = array[st_idx];
	csvloadtype s = 0.0;
	csvloadtype delta = 0.0;
	int nan_count = 0;
	for (size_t i = st_idx+1; i < ed_idx; i++) {
		if (isnan(array[i])) {
			nan_count++;
			continue;
		}
		delta = array[i] - m;
		m = m + delta / (i - st_idx + 1 - nan_count);
		s = s + delta * (array[i] - m);
	}

	return s / (ed_idx - st_idx - 1);
}


double var_at(arraytype* array, size_t st_idx, size_t ed_idx, bool ignore_nan) {
	if (ignore_nan)
		return var_d_ignore_nan(array->array, st_idx, ed_idx);
	else
		return var_d(array->array, st_idx, ed_idx);
}


double std_at(arraytype* array, size_t st_idx, size_t ed_idx, bool ignore_nan) {
	return sqrt(var_at(array, st_idx, ed_idx, ignore_nan));
}


arraytype* ma_at(arraytype* array, size_t period) {
	arraytype* result = allocate_array(array->length); // this creates an empty array!
	size_t i = 0;
	for (i = 0; i < period - 1; i++) {
		push_element(&result, NAN, false);
	}
	push_element(&result, mean_at(array, 0, period, false), false);  // first element
	for (i = period; i < array->length; i++) { // online algorithm here
		csvloadtype elt = result->array[i - 1] + (array->array[i] - array->array[i - period]) / period;
		push_element(&result, elt, false);
	}
	return result;
}


arraytype* rsi_at(arraytype* array, size_t period) {
	size_t i = 0;
	arraytype* result = allocate_array(array->length);
	for (i = 0; i < period; i++) push_element(&result, NAN, false);

	double up_a = 0.0;
	double dn_a = 0.0;
	for (i = 1; i <= period; ++i) {
		up_a += max(array->array[i] - array->array[i - 1], 0.0);
		dn_a += max(array->array[i - 1] - array->array[i], 0.0);
	}
	up_a /= period;
	dn_a /= period;
	double rs = (dn_a == 0.0) ? 1e8 : up_a / dn_a;
	push_element(&result, 100.0 - 100.0 / (1 + rs), false);

	for (i = period + 1; i < array->length; ++i) {
		double up = max(array->array[i] - array->array[i - 1], 0.0);
		double dn = max(array->array[i - 1] - array->array[i], 0.0);
		up_a = ((period - 1) * up_a + up) / period;
		dn_a = ((period - 1) * dn_a + dn) / period;
		rs = (dn_a == 0.0) ? 1e8 : up_a / dn_a;
		push_element(&result, 100.0 - 100.0 / (1 + rs), false);
	}

	return result;
}

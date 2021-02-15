#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>


#include "utils/mktapi/mktapi.h"
#include "utils/conv.h"
#include "utils/arrays/arrays.h"
#include "utils/analytics/analytics.h"
#include "utils/strategies/rsistrategy.h"
#include "utils/debugh/debug.h"


static char FILENAME[] = "..\\data\\spy.csv";


void print_intro() {
	printf("\n");
	printf("Strategy Tester!\n");
	printf("\n");
}


int backtest(void) {
	print_intro();
	
	int ma_long_period = 200;
	int ma_short_period = 5;
	int rsi_period = 2;
	double starting_capital = 10000.0;

	arraytype* open = allocate_array(INITIALLOADSIZE);
	arraytype* high = allocate_array(INITIALLOADSIZE);
	arraytype* low = allocate_array(INITIALLOADSIZE);
	arraytype* close = allocate_array(INITIALLOADSIZE);

	size_t data_length = loadcsv(FILENAME, &open, &high, &low, &close, 1);

	arraytype* returns = diff_at(close, 1);
	arraytype* ma200 = ma_at(close, ma_long_period);
	arraytype* ma5 = ma_at(close, ma_short_period);
	arraytype* rsi2 = rsi_at(close, rsi_period);

	csvloadtype mean_return = mean_at(returns, 0, returns->length, true);
	csvloadtype std_return = std_at(returns, 0, returns->length, true);


	printf(" Mean close returns are       : %.7f\n", mean_return);
	printf(" Std of close returns are     : %.7f\n", std_return);
	printf(" Sharpe Ratio of close returns: %.7f\n", mean_return / std_return);
	printf(" Final MA200 is:              : %.7f\n", ma200->array[ma200->length - 1]);
	printf(" Final RSI 2 is:              : %.7f\n", rsi2->array[rsi2->length - 1]);


	arraytype* cash = allocate_array(close->length);
	fill_array(cash, starting_capital, close->length);

	arraytype* posn = allocate_array(close->length);
	fill_array(posn, 0.0, close->length);

	size_t start_index = (size_t) ma_long_period - 1;

	// This is where the timing happens *******************************************
	double a = get_time();
	int number_runs = 100000;
	for (int i = 0; i < number_runs; i++) {
		run_rsi_strategy(close, ma200, ma5, rsi2, 20.0, start_index, posn, cash);
	}
	double b = get_time();
	printf("time in seconds: %.20f\n", (b - a) / number_runs);
	// *****************************************************************************

	arraytype* pl = deep_copy_array(cash);
	add_array(pl, mmul_array(posn, close));

	printf("Final PL Value                 : %.7f\n", pl->array[pl->length - 1]);

	// Freeing up all allocated memory.
	free_array(open);
	free_array(high);
	free_array(low);
	free_array(close);
	free_array(returns);
	free_array(ma200);
	free_array(ma5);
	free_array(rsi2);
	free_array(posn);
	free_array(cash);
	free_array(pl);

	return 0;
}


void main(int argc, char* argv[]) {
	int res = backtest();
}

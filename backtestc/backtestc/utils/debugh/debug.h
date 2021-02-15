#pragma once

#define DEBUG_H

/*
	Timer function.

	Usage:
		start = get_time();
		<CODE TO PROFILE>
		end = get_time();
		printf("seconds = %f", end - start);
*/
double get_time();
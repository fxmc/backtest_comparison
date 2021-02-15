#include <windows.h>


/*
	Timer function using windows specific QueryPerformance.  Won't work on other operating systems.
*/
double get_time()
{
	LARGE_INTEGER t, f;
	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&f);
	return (double)t.QuadPart / (double)f.QuadPart;
}
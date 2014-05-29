#include <Windows.h>
#include <iostream>
#include <time.h>
#include "Time.h"

static double g_freq;
static bool g_timerInitialised = false;

double Time::getTime()
{
	if(!g_timerInitialised)
	{
		LARGE_INTEGER lInt;

		if(!QueryPerformanceFrequency(&lInt))
		{
			std::cerr << "failed timer initialisation" << std::endl;
		}

		g_freq = double(lInt.QuadPart);
		g_timerInitialised = true;
	}

	LARGE_INTEGER lInt;

	if(!QueryPerformanceCounter(&lInt))
	{
		std::cerr << "failed in get time" << std::endl;
	}

	return double(lInt.QuadPart) / g_freq;
}
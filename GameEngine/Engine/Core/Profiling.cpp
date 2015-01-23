#include <iostream>

#include "Profiling.h"
#include "Time.h"


ProfileTimer::ProfileTimer()
{
	m_numInvocations = 0;
	m_totalTime = 0;
	m_startTime = 0;
}

void ProfileTimer::startInvocation()
{
	m_startTime = Time::getTime();
}

void ProfileTimer::stopInvocation()
{
	if(m_startTime == 0)
	{
		std::cout << "stopInvocation called without startInvocation" << std::endl;
	}

	m_numInvocations++;
	m_totalTime += (Time::getTime() - m_startTime);
	m_startTime = 0;
}

double ProfileTimer::displayAndReset(const std::string& message, double inputDivisor)
{
	double divisor = inputDivisor;

	if(divisor == 0)
	{
		divisor = m_numInvocations;
	}

	double time;

	if(divisor == 0)
	{
		time = 0;
	}
	else
	{
		time = (1000.0 * m_totalTime) / (double)divisor;
	}

	std::cout << message << time << " ms" << std::endl;
	m_numInvocations = 0;
	m_totalTime = 0.0;

	return time;
}

ProfileTimer ProfileTimers::inputTimer__;
ProfileTimer ProfileTimers::updateTimer_;
ProfileTimer ProfileTimers::renderTimer_;
ProfileTimer ProfileTimers::wSyncTimer__;
ProfileTimer ProfileTimers::wUpdateTimer;
ProfileTimer ProfileTimers::swapBufTimer;
ProfileTimer ProfileTimers::sleepTimer__;
ProfileTimer ProfileTimers::physicsTimer;
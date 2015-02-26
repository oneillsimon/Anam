#include <iostream>

#include "Profiling.h"
#include "Time.h"

int ProfileTimer::m_longestLabel = 0;

ProfileTimer::ProfileTimer(const std::string& label) :
	m_label(label)
{
	m_numInvocations = 0;
	m_totalTime = 0;
	m_startTime = 0;

	if(m_label.length() > (unsigned)m_longestLabel)
	{
		m_longestLabel = m_label.length();
	}
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

double ProfileTimer::displayAndReset(double inputDivisor)
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
	

	std::cout << Util::formatToLength(m_label, m_longestLabel, " ") << " - " << Util::formatToLength(std::to_string(time), 5) << " ms" << std::endl;
	m_numInvocations = 0;
	m_totalTime = 0.0;

	return time;
}

ProfileTimer ProfileTimers::inputTimer__("Input");
ProfileTimer ProfileTimers::updateTimer_("Update");
ProfileTimer ProfileTimers::physicsTimer("Physics");
ProfileTimer ProfileTimers::physRefTimer(" >Phys Ref Update");
ProfileTimer ProfileTimers::renderTimer_("Render Total");
ProfileTimer ProfileTimers::rnderGeneral(" >General");
ProfileTimer ProfileTimers::lightTimer__(" >Active Light");
ProfileTimer ProfileTimers::shadowTimer_(" >ShadowMap");
ProfileTimer ProfileTimers::wSyncTimer__("WinSync");
ProfileTimer ProfileTimers::wUpdateTimer("WinUpdate");
ProfileTimer ProfileTimers::swapBufTimer("Buff Swap");
ProfileTimer ProfileTimers::sleepTimer__("Sleep");
#ifndef PROFILING_H
#define PROFILING_H

#include <string>

class ProfileTimer
{
private:
	int m_numInvocations;
	double m_totalTime;
	double m_startTime;

public:
	ProfileTimer();

	void startInvocation();
	void stopInvocation();
	double displayAndReset(const std::string& message, double divisor = 0.0);
};

#endif
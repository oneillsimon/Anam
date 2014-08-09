#ifndef PROFILING_H
#define PROFILING_H

#include <string>

#define PROFILING_DISABLE_MESH_DRAWING 0
#define PROFILING_DISABLE_SHADING 0
#define PROFILING_SET_1x1_VIEWPORT 0
#define PROFILING_SET_2x2_TEXTURE 0

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
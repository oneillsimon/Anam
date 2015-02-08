#ifndef PROFILING_H
#define PROFILING_H

#include <string>

#include "Util.h"

#define PROFILING_DISABLE_MESH_DRAWING 0
#define PROFILING_DISABLE_SHADING 0
#define PROFILING_SET_1x1_VIEWPORT 0
#define PROFILING_SET_2x2_TEXTURE 0

class ProfileTimer
{
private:
	static int m_longestLabel;

	int m_numInvocations;
	double m_totalTime;
	double m_startTime;
	std::string m_label;

public:
	ProfileTimer(const std::string& label);

	void startInvocation();
	void stopInvocation();
	double displayAndReset(double divisor = 0.0);
};

class ProfileTimers
{
public:
	static ProfileTimer inputTimer__;
	static ProfileTimer updateTimer_;
	static ProfileTimer renderTimer_;
	static ProfileTimer rnderGeneral;
	static ProfileTimer lightTimer__;
	static ProfileTimer shadowTimer_;
	static ProfileTimer wSyncTimer__;
	static ProfileTimer wUpdateTimer;
	static ProfileTimer swapBufTimer;
	static ProfileTimer sleepTimer__;
	static ProfileTimer physicsTimer;
	static ProfileTimer physRefTimer;
};

#endif
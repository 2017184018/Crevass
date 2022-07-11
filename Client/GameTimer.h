//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************
#include "Singleton.h"

const ULONG MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer : public TemplateSingleton<GameTimer>
{
public:
	GameTimer();

	float TotalTime()const; // in seconds
	float DeltaTime()const; // in seconds
	unsigned long FrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick(const float fLockFPS);  // Call every frame.

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	float							m_fFrameTime[MAX_SAMPLE_COUNT];
	ULONG							m_nSampleCount;

	unsigned long					m_nCurrentFrameRate;
	unsigned long					m_nFramesPerSecond;
	float							m_fFPSTimeElapsed;

	bool mStopped;
};

#endif // GAMETIMER_H
#pragma once

class CTimer
{
private:
	int m_nFps;
	int m_nCount;

	unsigned long m_unStartTime;

	INT64 m_nFrequency; 
	float m_fTicksPerMs; 
	INT64 m_nTickStartTime; 
	float m_fFrameTime;
public:
	CTimer();
	~CTimer();

	void UpdateTimer();
	int  GetFps();

	float GetTickTime();
};
#pragma once
#include "SingleTon.h"

class CTimer:public CSingleTonT<CTimer>
{
	friend CSingleTonT;
private:
	int m_nFps;
	int m_nCount;

	unsigned long m_unStartTime;

	INT64 m_nFrequency; 
	float m_fTicksPerMs; 
	INT64 m_nTickStartTime; 
	float m_fFrameTime;
public:
	void UpdateTimer();
	int  GetFps();

	float GetTickTime();
protected:
	CTimer();
	~CTimer();
private:
	CTimer(CTimer&) {}
};
#include "stdafx.h"
#include "Timer.h"

#pragma comment(lib,"winmm.lib")

#include <windows.h>
#include <mmsystem.h>

CTimer::CTimer():
	m_nFps(0),m_nCount(0),m_unStartTime(0),
	m_nFrequency(0),m_fTicksPerMs(0.0f),m_nTickStartTime(0),m_fFrameTime(0.0f)
{
	m_unStartTime = timeGetTime();

	QueryPerformanceFrequency((LARGE_INTEGER*)&m_nFrequency);
	
	m_fTicksPerMs = ((float)m_nFrequency / 1000.f);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_nTickStartTime);
}

CTimer::~CTimer()
{
}

void CTimer::UpdateTimer()
{
	//update Tick

	INT64 nCurrentTime = 0;
	float fTimeDifference = 0.0f;

	QueryPerformanceCounter((LARGE_INTEGER*)&nCurrentTime);

	fTimeDifference = (float)(nCurrentTime - m_nTickStartTime);

	m_fFrameTime = fTimeDifference / m_fTicksPerMs;

	m_nTickStartTime = nCurrentTime;

	//update FPS

	m_nCount++;

	UINT unGetTime = timeGetTime();

	if (unGetTime >= (m_unStartTime + 1000))
	{
		m_nFps = m_nCount;
		m_nCount = 0;

		m_unStartTime = unGetTime;
	}
}

int CTimer::GetFps()
{
	return m_nFps;
}

float CTimer::GetTickTime()
{
	return m_fFrameTime;
}

#include "stdafx.h"
#include "CpuChecker.h"

CCpuChecker::CCpuChecker():
	m_bCanReadCpu(true),
	m_hQuery(nullptr),
	m_hCounter(nullptr),
	m_unLastSampleTime(0),
	m_nCpuUsage(0)
{
	PDH_STATUS status;

	status = PdhOpenQuery(NULL, 0, &m_hQuery);

	if (status != ERROR_SUCCESS)
		m_bCanReadCpu = false;

	status = PdhAddCounter(m_hQuery, _T("\\Processor(_Total)\\% processer time"), 0, &m_hCounter);

	if (status != ERROR_SUCCESS)
		m_bCanReadCpu = false;

	m_unLastSampleTime = GetTickCount();
}

CCpuChecker::~CCpuChecker()
{
	if (m_bCanReadCpu)
		PdhCloseQuery(m_hQuery);
}

void CCpuChecker::Update()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_bCanReadCpu)
	{
		if ((m_unLastSampleTime + 1000) < GetTickCount())
		{
			m_unLastSampleTime = GetTickCount();

			PdhCollectQueryData(m_hQuery);

			PdhGetFormattedCounterValue(m_hCounter, PDH_FMT_LONG, NULL, &value);

			m_nCpuUsage = value.longValue;
		}
	}
}

int CCpuChecker::GetCpuPercentage()
{
	int nUsage = 0;

	if (m_bCanReadCpu)
	{
		nUsage = m_nCpuUsage;
	}

	return nUsage;
}

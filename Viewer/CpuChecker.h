#pragma once

#pragma comment(lib,"pdh.lib")

#include <Pdh.h>
#include "SingleTon.h"

class CCpuChecker:public CSingleTonT<CCpuChecker>
{
	friend CSingleTonT;
private:
	bool		m_bCanReadCpu;
	HQUERY		m_hQuery;
	HCOUNTER	m_hCounter;
	unsigned long m_unLastSampleTime;
	long		m_nCpuUsage;
public:
	void	Update();
	int		GetCpuPercentage();
protected:
	CCpuChecker();
	~CCpuChecker();
private:
	CCpuChecker(CCpuChecker&) {}
};
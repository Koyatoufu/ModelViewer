#pragma once

template<class T>
class CSingleTonT
{
protected:
	static T* ms_pInstance;
protected:
	CSingleTonT() {}
	virtual ~CSingleTonT() {}
public:
	static T* CreateInstance()
	{
		if (ms_pInstance)
			return ms_pInstance;

		ms_pInstance = new T();

		return ms_pInstance;
	}

	static T* GetInstance()
	{
		return ms_pInstance;
	}

	static void ReleaseInstnace()
	{
		if (ms_pInstance)
		{
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}
};

template<class T>
T* CSingleTonT<T>::ms_pInstance = NULL;
#pragma once

template<typename T>
class CSingleton
{
private:
	typedef void(*DESTROY)(void);

private:
	static T*	m_This;

public:
	static T* GetInst()
	{
		if (nullptr == m_This)
		{
			m_This = new T;
		}
		return m_This;
	}

	static void Destroy()
	{
		if (nullptr != m_This)
		{
			delete m_This;
			m_This = nullptr;
		}
	}

protected:
	CSingleton()
	{
		// 프로그램 종료될때 자동으로 호출할 함수 등록
		atexit((DESTROY)&CSingleton<T>::Destroy);
	}
	CSingleton(const CSingleton& _other) = delete;
	~CSingleton()
	{
	}
};

template<typename T>
T* CSingleton<T>::m_This = nullptr;

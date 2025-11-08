#pragma once

template<typename T>
class Ptr
{
private:
	T* m_Asset;

public:
	void operator = (T* _Asset)
	{
		if (m_Asset != _Asset)
		{
			if (m_Asset != nullptr)
				m_Asset->Release();
			m_Asset = _Asset;
			if (m_Asset != nullptr)
				m_Asset->AddRef();
		}
	}

	void operator = (const Ptr<T>& _Other)
	{
		operator=(_Other.m_Asset);
	}

	T* operator->()
	{
		return m_Asset;
	}

public:
	Ptr()
		: m_Asset(nullptr)
	{
	}

	Ptr(T* _Asset)
		: m_Asset(_Asset)
	{
		if (m_Asset != nullptr)
			m_Asset->AddRef();
	}

	Ptr(const Ptr<T>& _Other)
		: Ptr(_Other.m_Asset)
	{
	}

	~Ptr()
	{
		if (m_Asset != nullptr)
			m_Asset->Release();
	}
};

#pragma once

// ���ø����̼� ��� �Ѱ� �Ŵ���
class CEngine
{
private:
	HWND		m_hMainWnd;
	POINT		m_Resolution;

public:
	int init(HWND _hWnd, POINT _Resolution);

	static  CEngine* GetInst()
	{
		static CEngine mgr;
		return &mgr;
	}
private:
	CEngine();
	CEngine(const CEngine& _origin) = delete;
};

#pragma once

// 어플리케이션 기능 총괄 매니저
class CEngine
{
private:
	HWND		m_hMainWnd;
	POINT		m_Resolution;

public:
	int init(HWND _hWnd, POINT _Resolution);
	void progress();

	static  CEngine* GetInst()
	{
		static CEngine mgr;
		return &mgr;
	}
private:
	CEngine();
	CEngine(const CEngine& _origin) = delete;
};

#pragma once

// 어플리케이션 기능 총괄 매니저
class CEngine
	: public CSingleton<CEngine>
{
	SINGLE(CEngine)
private:
	HWND		m_hMainWnd;
	POINT		m_Resolution;

public:
	HWND GetMainWnd() { return m_hMainWnd; }
	int init(HWND _hWnd, POINT _Resolution);
	void progress();
};

#include "pch.h"
#include "CTimeMgr.h"
#include "CEngine.h"

CTimeMgr::CTimeMgr()
	: m_llFrequency{}
	, m_llCurCount{}
	, m_llPrevCount{}
	, m_DT(0.f)
	, m_AccTime(0.f)
	, m_Time(0.f)
	, m_FrmCount(0)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	// 초당 카운트 수
	QueryPerformanceFrequency(&m_llFrequency);
	// 컴퓨터가 켜져있는동안 세어진 카운트. 카운터가 PC켤때 0으로 초기화됨
	QueryPerformanceCounter(&m_llCurCount);
	m_llPrevCount = m_llCurCount;
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_llCurCount);
	// 이전프레임의 TimeMgr의 tick 시작 ~ 현재 프레임의 TimeMgr tick 시작 까지의 간격
	m_DT = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount;

	m_Time += m_DT;
	m_AccTime += m_DT;
	++m_FrmCount;
	if (m_AccTime > 1.f)
	{
		m_AccTime -= 1.f;
		HWND hMainWnd = CEngine::GetInst()->GetMainWnd();
		wchar_t szText[256] = {};
		swprintf_s(szText, L"FPS: %d, DeltaTime : %f", m_FrmCount, m_DT);
		SetWindowText(hMainWnd, szText);
		m_FrmCount = 0;
	}
}

#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"

#include "Temp.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
	, m_Resolution{}
{
}

CEngine::~CEngine()
{
	TempRelease();
}

int CEngine::init(HWND _hWnd, POINT _Resolution)
{
	m_hMainWnd = _hWnd;
	m_Resolution = _Resolution;

	// 렌더링 영역 크기 < 실제 Window 크기
	RECT rt = { 0, 0, m_Resolution.x, m_Resolution.y };
	// 윈도우에서 기본적으로 제공되는 테두리, 타이틀바 등의 크기 계산
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	// DirectX11
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_Resolution)))
	{
		MessageBox(m_hMainWnd, L"Device 초기화 실패", L"엔진 초기화 실패", MB_OK);
		return E_FAIL;
	}


	// Manager Init
	CTimeMgr::GetInst()->init();

	if (FAILED(TempInit()))
	{
		MessageBox(m_hMainWnd, L"Device 초기화 실패", L"TempInit 초기화 실패", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CEngine::progress()
{
	// ============
	// Manager Tick
	// ============
	CTimeMgr::GetInst()->tick();

	// Object Tick
	TempTick();

	// ============
	// Rendering
	// ============
	// ClearRT
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	// Object Render
	TempRender();

	// Present
	CDevice::GetInst()->Present();
}

#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

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

	// ������ ���� ũ�� < ���� Window ũ��
	RECT rt = { 0, 0, m_Resolution.x, m_Resolution.y };
	// �����쿡�� �⺻������ �����Ǵ� �׵θ�, Ÿ��Ʋ�� ���� ũ�� ���
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	// DirectX11
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_Resolution)))
	{
		MessageBox(m_hMainWnd, L"Device �ʱ�ȭ ����", L"���� �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(TempInit()))
	{
		MessageBox(m_hMainWnd, L"Device �ʱ�ȭ ����", L"TempInit �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CEngine::progress()
{
	// Level->tick();
	TempTick();

	// ClearRT
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	// Level->render();
	TempRender();
	CDevice::GetInst()->Present();
}

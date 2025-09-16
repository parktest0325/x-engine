#pragma once

// DX ���� �Ŵ���
class CDevice
{
private:
	// ������ �ϰ��� �����ų �����츦 �˾ƾ���
	HWND					m_hMainWnd;
	// ������ �������� ũ�⺸�� ���� �ػ󵵷� �� �� ����. �Ϲ������δ� ��ġ��Ŵ
	// �޸𸮻��� �������� �̹����ؽ��� �ػ�
	POINT					m_RenderResolution;

	ID3D11Device*			m_Device;		// GPU �޸� �Ҵ�, Dx11 ���� ��ü(�ؽ���, ����, �� ��) ���� 
	ID3D11DeviceContext*	m_Context;		// ������ ���ҽ��� GPU ������ ���� �׷��Ƚ� ���

	IDXGISwapChain*			m_SwapChain;	// ���� Ÿ�� ���۸� �����ϸ鼭 ȭ�鿡 ���� ����� �Խ�

	ID3D11Texture2D*		m_RenderTargetTex;
	ID3D11RenderTargetView* m_RTV;
	ID3D10Texture2D*		m_DepthStencilTex;
	ID3D11DepthStencilView* m_DSV;

public:
	static CDevice* GetInst()
	{
		static CDevice mgr;
		return &mgr;
	}

	int init(HWND _hWnd, POINT _Resolution);

private:
	CDevice();
	CDevice(const CDevice& _other) = delete;

public:
	~CDevice();
};


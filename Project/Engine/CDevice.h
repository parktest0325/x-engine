#pragma once

// DX ���� �Ŵ���
class CDevice
{
private:
	// ������ �ϰ��� �����ų �����츦 �˾ƾ���
	HWND							m_hMainWnd;
	// ������ �������� ũ�⺸�� ���� �ػ󵵷� �� �� ����. �Ϲ������δ� ��ġ��Ŵ
	// �޸𸮻��� �������� �̹����ؽ��� �ػ�
	POINT							m_RenderResolution;

	ComPtr<ID3D11Device>			m_Device;		// GPU �޸� �Ҵ�, Dx11 ���� ��ü(�ؽ���, ����, �� ��) ���� 
	ComPtr<ID3D11DeviceContext>		m_Context;		// ������ ���ҽ��� GPU ������ ���� �׷��Ƚ� ��� ���

	ComPtr<IDXGISwapChain>			m_SwapChain;	// ���� Ÿ�� ���۵��� �����ϸ鼭 ȭ�鿡 ���� ����� �Խ�

	ComPtr<ID3D11Texture2D>			m_RenderTargetTex;
	ComPtr<ID3D11RenderTargetView>	m_RTV;
	ComPtr<ID3D11Texture2D>			m_DepthStencilTex;
	ComPtr<ID3D11DepthStencilView>	m_DSV;

public:
	static CDevice* GetInst()
	{
		static CDevice mgr;
		return &mgr;
	}

	int init(HWND _hWnd, POINT _Resolution);
	void ClearTarget(float(&_ArrColor)[4]);
	void Present() { m_SwapChain->Present(0, 0); }

private:
	int CreateSwapChain();
	int CreateView();

private:
	CDevice();
	CDevice(const CDevice& _other) = delete;

public:
	~CDevice();
};


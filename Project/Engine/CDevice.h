#pragma once

// DX 전담 매니저
class CDevice
{
private:
	// 렌더링 하고나면 송출시킬 윈도우를 알아야함
	HWND							m_hMainWnd;
	// 게임은 윈도우의 크기보다 낮은 해상도로 할 수 있음. 일반적으로는 일치시킴
	// 메모리상의 렌더링용 이미지텍스쳐 해상도
	POINT							m_RenderResolution;

	ComPtr<ID3D11Device>			m_Device;		// GPU 메모리 할당, Dx11 관련 객체(텍스쳐, 버퍼, 뷰 등) 생성 
	ComPtr<ID3D11DeviceContext>		m_Context;		// 생성된 리소스로 GPU 렌더링 관련 그래픽스 명령 기능

	ComPtr<IDXGISwapChain>			m_SwapChain;	// 렌더 타겟 버퍼들을 소유하면서 화면에 최종 장면을 게시

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


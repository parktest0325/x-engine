#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
	: m_hMainWnd(nullptr)
	, m_RenderResolution{}
	, m_Device(nullptr)
	, m_Context(nullptr)
	, m_SwapChain(nullptr)
	, m_RenderTargetTex(nullptr)
	, m_RTV(nullptr)
	, m_DepthStencilTex(nullptr)
	, m_DSV(nullptr)
{

}

CDevice::~CDevice()
{
	m_Device->Release();
	m_Context->Release();
	m_SwapChain->Release();

	m_RenderTargetTex->Release();
	m_RTV->Release();
	m_DepthStencilTex->Release();
	m_DSV->Release();
}

int CDevice::init(HWND _hWnd, POINT _Resolution)
{
	m_hMainWnd = _hWnd;
	m_RenderResolution = _Resolution;

	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

	// D3D_DRIVER_TYPE_HARDWARE : DXD11 GPU로 사용할거다. 여기서 실패하면 CPU로 렌더링하게됨
	// D3D11_CREATE_DEVICE_DEBUG : D3D11 사용하면서 디버그 로그 출력 
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
					nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION,
					&m_Device, &level, &m_Context);

	if (FAILED(CreateSwapChain()))
	{
		return E_FAIL;
	}
	if (FAILED(CreateView()))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC Desc = {};

	// SwapChain이 만들어질 때 버퍼 출력 설정
	Desc.BufferCount = 1;    // 백버퍼 한개
	Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;    // 스왑체인이 비트블록전송 방식 사용
	Desc.BufferDesc.Width = (UINT)m_RenderResolution.x;
	Desc.BufferDesc.Height = (UINT)m_RenderResolution.y;
	Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// RefreshRate = 60/1
	Desc.BufferDesc.RefreshRate.Denominator = 1;
	Desc.BufferDesc.RefreshRate.Numerator = 60;
	Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;  // 정의하지 않음
	Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // 버퍼의 용도

	// SwapChain이 화면을 게시(Present)할 때 출력 목적지 윈도우
	Desc.OutputWindow = m_hMainWnd;   // 버퍼를 출력할 윈도우핸들
	Desc.Windowed = true;

	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	if (FAILED(pFactory->CreateSwapChain(m_Device, &Desc, &m_SwapChain)))
	{
		pDXGIDevice->Release();
		pAdapter->Release();
		pFactory->Release();
		return E_FAIL;
	}

	pDXGIDevice->Release();
	pAdapter->Release();
	pFactory->Release();

	return S_OK;
}

int CDevice::CreateView()
{
	// 1. 스왑체인의 RenderTarget Texture(백버퍼)를 가져온다.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_RenderTargetTex);

	// 2. 사용하려면 RenderTargetView를 만들어야한다.
	m_Device->CreateRenderTargetView(m_RenderTargetTex, nullptr, &m_RTV);

	// 3. DepthStencil용 Textrue 를 제작
	D3D11_TEXTURE2D_DESC Desc = {};
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 3byte 뎁스, 1byte 스텐실
	Desc.Width = (UINT)m_RenderResolution.x;
	Desc.Height = (UINT)m_RenderResolution.y;
	Desc.ArraySize = 1;

	Desc.CPUAccessFlags = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_Device->CreateTexture2D(&Desc, nullptr, &m_DepthStencilTex);

	// 4. DepthStencilView 얻어오기
	m_Device->CreateDepthStencilView(m_DepthStencilTex, nullptr, &m_DSV);

	// RenderTarget, DepthStencil 을 출력으로 지정
	m_Context->OMSetRenderTargets(1, &m_RTV, m_DSV);

	return S_OK;
}

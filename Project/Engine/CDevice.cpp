#include "pch.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CDevice::CDevice()
	: m_hMainWnd(nullptr)
	, m_RenderResolution{}
{
}

CDevice::~CDevice()
{
	// 컴파일러가 m_CB의 타입과 사이즈로 템플릿으로 함수를 만듦
	Safe_Del_Array(m_CB);
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
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION,
			m_Device.GetAddressOf(), &level, m_Context.GetAddressOf())))
		return E_FAIL;

	if (FAILED(CreateSwapChain()))
		return E_FAIL;
	if (FAILED(CreateView()))
		return E_FAIL;

	// ViewPort 로 윈도우에 보여질 영역설정
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_RenderResolution.x;
	viewport.Height = m_RenderResolution.y;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	// ViewPort 정보 세팅
	m_Context->RSSetViewports(1, &viewport);

	// 필요한 상수버퍼 생성
	if (FAILED(CreateConstBuffer()))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::ClearTarget(float(&_ArrColor)[4])
{
	m_Context->ClearRenderTargetView(m_RTV.Get(), _ArrColor);
	m_Context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
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

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter = nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr;

	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

int CDevice::CreateView()
{
	// 1. 스왑체인의 RenderTarget Texture(백버퍼)를 가져온다.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RenderTargetTex.GetAddressOf());

	// 2. 사용하려면 RenderTargetView를 만들어야한다.
	m_Device->CreateRenderTargetView(m_RenderTargetTex.Get(), nullptr, m_RTV.GetAddressOf());

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

	m_Device->CreateTexture2D(&Desc, nullptr, m_DepthStencilTex.GetAddressOf());

	// 4. DepthStencilView 얻어오기
	m_Device->CreateDepthStencilView(m_DepthStencilTex.Get(), nullptr, m_DSV.GetAddressOf());

	// RenderTarget, DepthStencil 을 출력으로 지정
	m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());

	return S_OK;
}

int CDevice::CreateConstBuffer()
{
	m_CB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
	m_CB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), CB_TYPE::TRANSFORM);

	return S_OK;
}

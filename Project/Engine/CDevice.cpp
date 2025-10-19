#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
	: m_hMainWnd(nullptr)
	, m_RenderResolution{}
{
}

CDevice::~CDevice()
{
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

	// D3D_DRIVER_TYPE_HARDWARE : DXD11 GPU�� ����ҰŴ�. ���⼭ �����ϸ� CPU�� �������ϰԵ�
	// D3D11_CREATE_DEVICE_DEBUG : D3D11 ����ϸ鼭 ����� �α� ��� 
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION,
			m_Device.GetAddressOf(), &level, m_Context.GetAddressOf())))
		return E_FAIL;

	if (FAILED(CreateSwapChain()))
		return E_FAIL;
	if (FAILED(CreateView()))
		return E_FAIL;

	// ViewPort �� �����쿡 ������ ��������
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_RenderResolution.x;
	viewport.Height = m_RenderResolution.y;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	// ViewPort ���� ����
	m_Context->RSSetViewports(1, &viewport);

	return S_OK;
}

void CDevice::ClearTarget(float(&_ArrColor)[4])
{
	m_Context->ClearRenderTargetView(m_RTV.Get(), _ArrColor);
}

int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC Desc = {};

	// SwapChain�� ������� �� ���� ��� ����
	Desc.BufferCount = 1;    // ����� �Ѱ�
	Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;    // ����ü���� ��Ʈ������� ��� ���
	Desc.BufferDesc.Width = (UINT)m_RenderResolution.x;
	Desc.BufferDesc.Height = (UINT)m_RenderResolution.y;
	Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// RefreshRate = 60/1
	Desc.BufferDesc.RefreshRate.Denominator = 1;
	Desc.BufferDesc.RefreshRate.Numerator = 60;
	Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;  // �������� ����
	Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // ������ �뵵

	// SwapChain�� ȭ���� �Խ�(Present)�� �� ��� ������ ������
	Desc.OutputWindow = m_hMainWnd;   // ���۸� ����� �������ڵ�
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
	// 1. ����ü���� RenderTarget Texture(�����)�� �����´�.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RenderTargetTex.GetAddressOf());

	// 2. ����Ϸ��� RenderTargetView�� �������Ѵ�.
	m_Device->CreateRenderTargetView(m_RenderTargetTex.Get(), nullptr, m_RTV.GetAddressOf());

	// 3. DepthStencil�� Textrue �� ����
	D3D11_TEXTURE2D_DESC Desc = {};
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 3byte ����, 1byte ���ٽ�
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

	// 4. DepthStencilView ������
	m_Device->CreateDepthStencilView(m_DepthStencilTex.Get(), nullptr, m_DSV.GetAddressOf());

	// RenderTarget, DepthStencil �� ������� ����
	m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());

	return S_OK;
}

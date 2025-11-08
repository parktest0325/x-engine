#include "pch.h"
#include "CGraphicShader.h"
#include "CDevice.h"

CGraphicShader::CGraphicShader()
	: CShader(ASSET_TYPE::GRAPHIC_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CGraphicShader::~CGraphicShader()
{
}

int CGraphicShader::CreateVertexShader(const wstring& _strFilePath, const string& _VSFunctName)
{
	if (FAILED(D3DCompileFromFile(_strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_VSFunctName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0,
		m_VSBlob.GetAddressOf(),
		m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			// 문법오류 등
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "버텍스 쉐이더 컴파일 오류", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"파일을 찾을 수 없습니다.", L"버텍스 쉐이더 컴파일 오류", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(),
		m_VSBlob->GetBufferSize(),
		nullptr, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}

	// 레이아웃 생성
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[2] = {};

	LayoutDesc[0].AlignedByteOffset = 0;
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// 사실 정점에서 포지션의 정보라 xyz 4byte 3개인데 어쩔수없이 이걸로 한다. 사이즈가 비;슷한걸로
	LayoutDesc[0].InputSlot = 0;						// 전달되는 버텍스버퍼가 몇번째 슬롯에 있는건지.. IA에 버텍스버퍼를 전달할때 사실 버텍스버퍼 말고도 여러 버퍼를 전달한다. 이때 몇번째 인덱스인지
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 이 슬롯은 정점데이터이다.
	LayoutDesc[0].InstanceDataStepRate = 0;
	LayoutDesc[0].SemanticName = "POSITION";			// 이 Layout의 ID를 부여한것
	LayoutDesc[0].SemanticIndex = 0;					// 같은 시멘틱 네임중에서 인덱스. 각 엘리멘트에서 16byte가 최대인데, 64byte짜리 엘리멘트를 표현하려면 4개로 나눠야하기 때문에 같은 시멘틱 이름을 가질 수 잇다. 그렇게 됐을때 인덱스를 나눔
	
	LayoutDesc[1].AlignedByteOffset = sizeof(Vtx::vPos);
	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[1].InputSlot = 0;
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;
	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;
	
	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 2,
		m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
		m_Layout.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicShader::CreatePixelShader(const wstring& _strFilePath, const string& _PSFunctName)
{
	if (FAILED(D3DCompileFromFile(_strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_PSFunctName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0,
		m_PSBlob.GetAddressOf(),
		m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "픽셀 쉐이더 컴파일 오류", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"파일을 찾을 수 없습니다.", L"픽셀 쉐이더 컴파일 오류", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(),
		m_PSBlob->GetBufferSize(),
		nullptr, m_PS.GetAddressOf())))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CGraphicShader::Binding()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);
	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);
}

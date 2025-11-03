#include "pch.h"
#include "Temp.h"
#include "CDevice.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

// 정점 정보를 저장하는 버퍼 포인터
ComPtr<ID3D11Buffer> g_VB;

// 버텍스쉐이더에서 사용할 정점을 가리키는 인덱스 버퍼
ComPtr<ID3D11Buffer> g_IB;

// 정점을 구성하는 레이아웃 정보
ComPtr<ID3D11InputLayout> g_Layout;

// 시스템메모리의 정점 정보
Vtx g_arrVtx[4] = {};

// Vertex Shader
ComPtr<ID3DBlob>			g_VSBlob;   // HLSL 컴파일 한 쉐이더코드 저장
ComPtr<ID3D11VertexShader>	g_VS;		// Vertex Shader 객체

// Pixel Shader
ComPtr<ID3DBlob>			g_PSBlob;
ComPtr<ID3D11PixelShader>	g_PS;

// Error Blob
ComPtr<ID3DBlob>			g_ErrBlob;



int TempInit()
{
	// 좌표는 그냥 수학적인 좌표평면계 처럼 왼쪽아래가 작다.
	// 0 -- 1
	// |  \ |
	// 3 -- 2
	g_arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	g_arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	g_arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	g_arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	g_arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	g_arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	g_arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	g_arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);


	// GPU 메모리로 옮겨야됨
	// 정점버퍼 생성 (GPU메모리에 생성됨)
	D3D11_BUFFER_DESC VBDesc = {};
	VBDesc.ByteWidth = sizeof(Vtx) * 6;				// 버퍼의 크기
	VBDesc.MiscFlags = 0;		// Not used
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 생성되는 버퍼의 용도. 나중에 버텍스버퍼를 받는 인풋어셈블러에서 에러가 발생하지 않게 지정
	// 생성된 이후에도 지속적으로 CPU에서 접근해서 수정될 수 있는 녀석이다.
	VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VBDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA SubDesc = {};    // 초기값
	SubDesc.pSysMem = g_arrVtx;
	if (FAILED(DEVICE->CreateBuffer(&VBDesc, &SubDesc, g_VB.GetAddressOf())))
	{
		return E_FAIL;
	}

	// 인덱스버퍼 생성
	// [0,2,3,0,1,2]
	UINT arrIdx[6] = { 0, 2, 3, 0, 1, 2 };
	D3D11_BUFFER_DESC IBDesc = {};
	IBDesc.ByteWidth = sizeof(UINT) * 6;
	IBDesc.MiscFlags = 0;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;		// 인덱스 버퍼용 플래그

	IBDesc.CPUAccessFlags = 0;						// 한번 생성한 이후 수정할 필요가 없음 (CPU에서는 RW 불가)
	IBDesc.Usage = D3D11_USAGE_DEFAULT;

	SubDesc = {};
	SubDesc.pSysMem = arrIdx;

	if (FAILED(DEVICE->CreateBuffer(&IBDesc, &SubDesc, g_IB.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 버텍스 쉐이더
	wchar_t szBuffer[256] = {};
	GetCurrentDirectory(256, szBuffer);
	size_t len = wcslen(szBuffer);
	for (int i = len - 1; i > 0; --i)
	{
		if (szBuffer[i] == '\\')
		{
			szBuffer[i] = '\0';
			break;
		}
	}
	wcscat_s(szBuffer, L"\\content\\shader\\std2d.fx");
	if (FAILED(D3DCompileFromFile(szBuffer, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS_Std2D", "vs_5_0", D3DCOMPILE_DEBUG, 0,
		g_VSBlob.GetAddressOf(),
		g_ErrBlob.GetAddressOf())))
	{
		if (nullptr != g_ErrBlob)
		{
			// 문법오류 등
			MessageBoxA(nullptr, (char*)g_ErrBlob->GetBufferPointer(), "버텍스 쉐이더 컴파일 오류", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"파일을 찾을 수 없습니다.", L"버텍스 쉐이더 컴파일 오류", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer(),
		g_VSBlob->GetBufferSize(),
		nullptr, g_VS.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 정점 레이아웃 정보 생성
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
		g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(),
		g_Layout.GetAddressOf())))
	{
		return E_FAIL;
	}

	
	// 픽셀 쉐이더
	if (FAILED(D3DCompileFromFile(szBuffer, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS_Std2D", "ps_5_0", D3DCOMPILE_DEBUG, 0,
		g_PSBlob.GetAddressOf(),
		g_ErrBlob.GetAddressOf())))
	{
		if (nullptr != g_ErrBlob)
		{
			MessageBoxA(nullptr, (char*)g_ErrBlob->GetBufferPointer(), "픽셀 쉐이더 컴파일 오류", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"파일을 찾을 수 없습니다.", L"픽셀 쉐이더 컴파일 오류", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer(),
		g_PSBlob->GetBufferSize(),
		nullptr, g_PS.GetAddressOf())))
	{
		return E_FAIL;
	}


	return S_OK;
}

void TempRelease()
{
}

void TempTick()
{
	float DT = CTimeMgr::GetInst()->GetDeltaTime();

	if (KEY_PRESSED(KEY::W))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_arrVtx[i].vPos.y += 1.f * DT;
		}
	}
	if (KEY_PRESSED(KEY::S))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_arrVtx[i].vPos.y -= 1.f * DT;
		}
	}
	if (KEY_PRESSED(KEY::D))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_arrVtx[i].vPos.x += 1.f * DT;
		}
	}
	if (KEY_PRESSED(KEY::A))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_arrVtx[i].vPos.x -= 1.f * DT;
		}
	}

	// SystemMemory -> GPU Memory
	// VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 이 옵션이 없으면 Map 함수에서 실패해서 tSub.pData에 nullptr 가 저장된다
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(g_VB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, g_arrVtx, sizeof(Vtx) * 4);
	CONTEXT->Unmap(g_VB.Get(), 0);
}

void TempRender()
{
	// InputAssembler 세팅
	UINT Stride = sizeof(Vtx);	// 버퍼에서 정점의 간격
	UINT Offset = 0;			// 렌더링할 정점의 버퍼상 오프셋. 일부만(몸통만) 렌더링하기 위해 오프셋을 지정할수도 있음 
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &Stride, &Offset);

	// 인덱스버퍼가 지금 UINT타입(4byte)이라서 R32_UINT 포맷으로 지정한다. 2byte하나가 인덱스 하나라면 포맷도 맞는 크기로 변경해야한다.
	CONTEXT->IASetIndexBuffer(g_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	CONTEXT->IASetInputLayout(g_Layout.Get());
	// 레스터라이저에서 토폴로지에 따라서 선택할 픽셀을 결정한다.
	// 삼각형이라고 해도 내부를 채우는 삼각형인지, 테두리만 픽셀쉐이더로 선택할건지를 결정해야하기 때문
	// TRIANGLELIST를 사용하면, 전달한 정점 리스트를 세개씩 끊어서 삼각형으로 인식하고 그 내부를 채우는 면 형태로 인식한다는 의미.
	CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 쉐이더 세팅
	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0);	// 픽셀쉐이더가 모든픽셀에 하나씩 적용되는데, 빨간색이 리턴되는 픽셀쉐이더로 구현되어있음
	// 뎁스스텐실스테이트,  블렌드스테이트 기본값 사용할것

	// 0에서부터 6개의 정점을 렌더링한다. 
	// CONTEXT->Draw(6, 0);
	CONTEXT->DrawIndexed(6, 0, 0);
}

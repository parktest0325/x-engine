#include "pch.h"
#include "Temp.h"
#include "CDevice.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"

#include "CMesh.h"

// Mesh
CMesh* g_RectMesh = nullptr;
CMesh* g_CircleMesh = nullptr;


// 상수버퍼(Constant Buffer) 물체의 위치, 크기, 회전량 정보를 전달하는 용도
ComPtr<ID3D11Buffer> g_CB;

// 정점을 구성하는 레이아웃 정보
ComPtr<ID3D11InputLayout> g_Layout;

// 시스템메모리의 정점 정보
Vtx g_arrVtx[4] = {};

// 물체 위치값
Vec3 g_ObjectPos;

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
	// ===========
	// Rect Mesh
	// ===========
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
	g_arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	UINT arrIdx[6] = { 0, 2, 3, 0, 1, 2 };

	g_RectMesh = new CMesh;
	g_RectMesh->Create(g_arrVtx, 4, arrIdx, 6);


	// ===========
	// Circle Mesh 
	// ===========
	vector<Vtx> vecVtx;
	// 원점
	Vtx v;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	float Radius = 0.5;
	UINT Slice = 60;
	float AngleStep = (2 * XM_PI) / Slice;

	float Angle = 0.f;
	// 정점을 하나 더 넣는데, 이건 인덱스 반복문 돌때 예외처리할 필요 없도록 하기 위함.
	for (UINT i = 0; i <= Slice; ++i)
	{
		Vtx v;
		// 임의의 반지름 R일때, 각도 세타에서 원과 만나는 점 = (R*Cos(세타), R*Sin(세타))
		v.vPos = Vec3(cosf(Angle) * Radius, sinf(Angle) * Radius, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

		vecVtx.push_back(v);

		Angle += AngleStep;
	}

	vector<UINT> vecIdx;
	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	g_CircleMesh = new CMesh;
	g_CircleMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());


	// 상수버퍼 생성
	D3D11_BUFFER_DESC CBDesc = {};

	CBDesc.ByteWidth = sizeof(tTransform);
	CBDesc.MiscFlags = 0;
	CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// 위치값이기 때문에 계속 수정해야함.
	CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CBDesc.Usage = D3D11_USAGE_DYNAMIC;

	// 초기값 없음
	if (FAILED(DEVICE->CreateBuffer(&CBDesc, nullptr, g_CB.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 버텍스 쉐이더
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += L"shader\\std2d.fx";

	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
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
	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
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

	g_ObjectPos = Vec3(0.f, 0.f, 0.f);

	return S_OK;
}

void TempRelease()
{
	if (g_RectMesh != nullptr)
		delete g_RectMesh;
	if (g_CircleMesh != nullptr)
		delete g_CircleMesh;
}

void TempTick()
{
	float DT = CTimeMgr::GetInst()->GetDeltaTime();

	if (KEY_PRESSED(KEY::W))
	{
		g_ObjectPos.y += DT;
	}
	if (KEY_PRESSED(KEY::S))
	{
		g_ObjectPos.y -= DT;
	}
	if (KEY_PRESSED(KEY::D))
	{
		g_ObjectPos.x += DT;
	}
	if (KEY_PRESSED(KEY::A))
	{
		g_ObjectPos.x -= DT;
	}

	// SystemMemory -> GPU Memory
	// VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 이 옵션이 없으면 Map 함수에서 실패해서 tSub.pData에 nullptr 가 저장된다
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(g_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	tTransform trans = {};
	trans.Position = g_ObjectPos;
	memcpy(tSub.pData, &trans, sizeof(tTransform));
	CONTEXT->Unmap(g_CB.Get(), 0);
}

void TempRender()
{
							// 레지스터 번호, 수, CPU상수버퍼
	CONTEXT->VSSetConstantBuffers(0, 1, g_CB.GetAddressOf());

	CONTEXT->IASetInputLayout(g_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 쉐이더 세팅
	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0);	// 픽셀쉐이더가 모든픽셀에 하나씩 적용되는데, 빨간색이 리턴되는 픽셀쉐이더로 구현되어있음
	// 뎁스스텐실스테이트,  블렌드스테이트 기본값 사용할것

	//g_RectMesh->render();
	g_CircleMesh->render();
}

#include "pch.h"
#include "Temp.h"
#include "CDevice.h"

// 정점 정보를 저장하는 버퍼 포인터
ComPtr<ID3D11Buffer> g_VB;

// 정점을 구성하는 레이아웃 정보
ComPtr<ID3D11InputLayout> g_Layout;

// 시스템메모리의 정점 정보
Vtx g_arrVtx[3] = {};

// Vertex Shader
ComPtr<ID3DBlob>			g_VBBlob;   // HLSL 컴파일 한 쉐이더코드 저장
ComPtr<ID3D11VertexShader>	g_VS;		// Vertex Shader 객체

// Pixel Shader
ComPtr<ID3DBlob>			g_PSBlob;
ComPtr<ID3D11PixelShader>	g_PS;


int TempInit()
{
	// 좌표는 그냥 수학적인 좌표평면계 처럼 왼쪽아래가 작다.
	//             0  (0, 1)
	//           /   \ 
	// (-1,-1)  2 --- 1  (1,-1)
	g_arrVtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	g_arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	g_arrVtx[1].vPos = Vec3(1.f, -1.f, 0.f);
	g_arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	g_arrVtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
	g_arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	// 정점버퍼 생성 (GPU메모리에 생성됨)
	D3D11_BUFFER_DESC VBDesc = {};
	VBDesc.ByteWidth = sizeof(Vtx) * 3;				// 버퍼의 크기
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

	// 버텍스 쉐이더

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
	
	//DEVICE->CreateInputLayout(LayoutDesc, 2, );
	

	return S_OK;

	// GPU 메모리로 옮겨야됨
}

void TempRelease()
{
}

void TempTick()
{
}

void TempRender()
{
}

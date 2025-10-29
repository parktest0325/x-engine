#include "pch.h"
#include "Temp.h"
#include "CDevice.h"

// 정점 정보를 저장하는 버퍼 포인터
ComPtr<ID3D11Buffer> g_VB;
Vtx g_arrVtx[3] = {};

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

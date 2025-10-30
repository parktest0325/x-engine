#include "pch.h"
#include "Temp.h"
#include "CDevice.h"

// ���� ������ �����ϴ� ���� ������
ComPtr<ID3D11Buffer> g_VB;
Vtx g_arrVtx[3] = {};

int TempInit()
{
	// ��ǥ�� �׳� �������� ��ǥ���� ó�� ���ʾƷ��� �۴�.
	//             0  (0, 1)
	//           /   \ 
	// (-1,-1)  2 --- 1  (1,-1)
	g_arrVtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	g_arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	g_arrVtx[1].vPos = Vec3(1.f, -1.f, 0.f);
	g_arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	g_arrVtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
	g_arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	// �������� ���� (GPU�޸𸮿� ������)
	D3D11_BUFFER_DESC VBDesc = {};
	VBDesc.ByteWidth = sizeof(Vtx) * 3;				// ������ ũ��
	VBDesc.MiscFlags = 0;		// Not used
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �����Ǵ� ������ �뵵. ���߿� ���ؽ����۸� �޴� ��ǲ��������� ������ �߻����� �ʰ� ����
	// ������ ���Ŀ��� ���������� CPU���� �����ؼ� ������ �� �ִ� �༮�̴�.
	VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VBDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA SubDesc = {};    // �ʱⰪ
	SubDesc.pSysMem = g_arrVtx;
	if (FAILED(DEVICE->CreateBuffer(&VBDesc, &SubDesc, g_VB.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;

	// GPU �޸𸮷� �Űܾߵ�
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

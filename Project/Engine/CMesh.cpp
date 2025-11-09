#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"

CMesh::CMesh()
	: CAsset(ASSET_TYPE::MESH)
	, m_VBDesc{}
	, m_VtxCount(0)
	, m_VtxSysMem(nullptr)
	, m_IBDesc{}
	, m_IdxCount(0)
	, m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (m_VtxSysMem != nullptr)
		delete[] m_VtxSysMem;
	if (m_IdxSysMem != nullptr)
		delete[] m_IdxSysMem;
}

int CMesh::Create(Vtx* _VtxSysMem, size_t _VtxCount, UINT* _IdxSysMem, size_t _IdxCount)
{
	// 정점버퍼 생성 (GPU메모리에 생성됨)
	m_VBDesc.ByteWidth = (UINT)(sizeof(Vtx) * _VtxCount);
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA SubDesc = {};
	SubDesc.pSysMem = _VtxSysMem;
	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &SubDesc, m_VB.GetAddressOf())))
	{
		return E_FAIL;
	}

	// 인덱스버퍼 생성
	// [0,2,3,0,1,2]
	m_IdxCount = (UINT)_IdxCount;
	m_IBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

	SubDesc = {};
	SubDesc.pSysMem = _IdxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &SubDesc, m_IB.GetAddressOf())))
	{
		return E_FAIL;
	}

	// SysMem 유지를 위한것. 외부에서 _VtxSysMem 이 유지된다는 보장을 할 수 없음.
	m_VtxSysMem = new Vtx[m_VtxCount];
	memcpy(m_VtxSysMem, _VtxSysMem, sizeof(Vtx) * m_VtxCount);
	m_IdxSysMem = new UINT[m_IdxCount];
	memcpy(m_IdxSysMem, _IdxSysMem, sizeof(UINT) * m_IdxCount);

	return S_OK;
}

void CMesh::Binding()
{
	// InputAssembler 세팅
	UINT Stride = sizeof(Vtx);	// 버퍼에서 정점의 간격
	UINT Offset = 0;			// 렌더링할 정점의 버퍼상 오프셋. 일부만(몸통만) 렌더링하기 위해 오프셋을 지정할수도 있음 
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &Stride, &Offset);

	// 인덱스버퍼가 지금 UINT타입(4byte)이라서 R32_UINT 포맷으로 지정한다. 2byte하나가 인덱스 하나라면 포맷도 맞는 크기로 변경해야한다.
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render()
{
	Binding();
	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

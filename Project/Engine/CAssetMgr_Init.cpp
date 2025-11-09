#include "pch.h"
#include "CAssetMgr.h"
#include "CPathMgr.h"


void CAssetMgr::init()
{
	CreateDefaultMesh();
	CreateDefaultTexture();
	CreateDefaultMaterial();
	CreateDefaultGraphicShader();
	CreateDefaultComputeShader();
}

void CAssetMgr::CreateDefaultMesh()
{
	Ptr<CMesh> pMesh = nullptr;
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// ===========
	// Rect Mesh
	// ===========
	// 좌표는 그냥 수학적인 좌표평면계 처럼 왼쪽아래가 작다.
	// 0 -- 1
	// |  \ |
	// 3 -- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	vecVtx.push_back(v);



	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	UINT arrIdx[6] = { 0, 2, 3, 0, 1, 2 };

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset<CMesh>(L"RectMesh", pMesh);

	vecVtx.clear();
	vecVtx.clear();

	// ===========
	// Circle Mesh 
	// ===========
	// 원점
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

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// TODO: 여기에서 문제가 발생하지 않나?
	// pMesh에 CircleMesh용 데이터를 넣으면 기존에 가리키던 Mesh를 Release할텐데..
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset<CMesh>(L"CircleMesh", pMesh);
}

void CAssetMgr::CreateDefaultTexture()
{

}

void CAssetMgr::CreateDefaultMaterial()
{

}

void CAssetMgr::CreateDefaultGraphicShader()
{

	Ptr<CGraphicShader> pShader = nullptr;

	// 버텍스 쉐이더, 픽셀 쉐이더 생성
	wstring strPath = CPathMgr::GetInst()->GetContentPath();

	pShader = new CGraphicShader;
	pShader->CreateVertexShader(strPath + L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(strPath + L"shader\\std2d.fx", "PS_Std2D");

	AddAsset<CGraphicShader>(L"Std2DShader", pShader);
}

void CAssetMgr::CreateDefaultComputeShader()
{

}

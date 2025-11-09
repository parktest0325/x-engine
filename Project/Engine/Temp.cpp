#include "pch.h"
#include "Temp.h"
#include "CDevice.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CConstBuffer.h"
#include "CGraphicShader.h"

// 물체의 위치, 크기, 회전
tTransform g_Trans = {};

int TempInit()
{





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
		g_Trans.Position.y += DT;
	}
	if (KEY_PRESSED(KEY::S))
	{
		g_Trans.Position.y -= DT;
	}
	if (KEY_PRESSED(KEY::D))
	{
		g_Trans.Position.x += DT;
	}
	if (KEY_PRESSED(KEY::A))
	{
		g_Trans.Position.x -= DT;
	}

	// SystemMemory -> GPU Memory
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_Trans);
	pCB->Binding();
}

void TempRender()
{
	// 쉐이더 세팅
	Ptr<CGraphicShader> pGS = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"Std2DShader");
	pGS->Binding();
	// 뎁스스텐실스테이트,  블렌드스테이트 기본값 사용할것

	// 메쉬 세팅
	//g_RectMesh->render();
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
	pRectMesh->render();
}

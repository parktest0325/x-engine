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

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

CGameObject* pObject = nullptr;
CGameObject* pObject2 = nullptr;


int TempInit()
{
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	pObject->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"Std2DShader"));

	pObject2 = new CGameObject;
	pObject2->AddComponent(new CTransform);
	pObject2->AddComponent(new CMeshRender);

	pObject2->Transform()->SetRelativeScale(Vec3(0.4f, 0.4f, 0.4f));

	pObject2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject2->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"Std2DShader"));
	return S_OK;
}

void TempRelease()
{
	delete pObject;
	delete pObject2;
}

void TempTick()
{
	pObject->tick();
	pObject2->tick();

	pObject->finaltick();
	pObject2->finaltick();
}

void TempRender()
{
	pObject->render();
	pObject2->render();
}

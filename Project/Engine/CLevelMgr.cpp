#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CGameObject.h"

#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"
#include "CAssetMgr.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (m_CurLevel != nullptr)
	{
		delete m_CurLevel;
	}
}

void CLevelMgr::init()
{
	m_CurLevel = new CLevel;


	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	pObject->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"Std2DShader"));

	CGameObject* pObject2 = new CGameObject;
	pObject2->AddComponent(new CTransform);
	pObject2->AddComponent(new CMeshRender);

	pObject2->Transform()->SetRelativeScale(Vec3(0.4f, 0.4f, 0.4f));

	pObject2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject2->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"Std2DShader"));

	m_CurLevel->AddObject(0, pObject);
	m_CurLevel->AddObject(0, pObject2);
}

void CLevelMgr::tick()
{
	if (m_CurLevel != nullptr)
	{
		m_CurLevel->tick();
		m_CurLevel->finaltick();
	}
}

void CLevelMgr::render()
{
	if (m_CurLevel != nullptr)
		m_CurLevel->render();
}

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

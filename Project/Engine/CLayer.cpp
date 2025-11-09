#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
	: m_vecObject{}
{
}

CLayer::~CLayer()
{
	Safe_Del_Vector(m_vecObject);
}

void CLayer::begin()
{
	for (CGameObject* obj : m_vecObject)
	{
		obj->begin();
	}
}

void CLayer::tick()
{
	for (CGameObject* obj : m_vecObject)
	{
		obj->tick();
	}
}

void CLayer::finaltick()
{
	for (CGameObject* obj : m_vecObject)
	{
		obj->finaltick();
	}
}

void CLayer::render()
{
	for (CGameObject* obj : m_vecObject)
	{
		obj->render();
	}
}


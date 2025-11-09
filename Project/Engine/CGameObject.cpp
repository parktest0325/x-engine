#include "pch.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CRenderComponent.h"
#include "CScript.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
{
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
}

void CGameObject::begin()
{
	for (CComponent* component : m_arrCom)
	{
		if (component != nullptr)
			component->begin();
	}

	for (CScript* script : m_vecScripts)
	{
		script->begin();
	}
}

void CGameObject::tick()
{
	for (CComponent* component : m_arrCom)
	{
		if (component != nullptr)
			component->tick();
	}
	for (CScript* script : m_vecScripts)
	{
		script->tick();
	}
}

void CGameObject::finaltick()
{
	for (CComponent* component : m_arrCom)
	{
		if (component != nullptr)
			component->finaltick();
	}
}

void CGameObject::render()
{
	if (m_RenderCom)
	{
		m_RenderCom->render();
	}
}

void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE type = _Component->GetComponentType();

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// 스크립트는 무한대로 넣을 수 있음
		m_vecScripts.push_back((CScript*)_Component);
	}
	else
	{
		// 이미 컴포넌트가 있는 경우
		assert(!m_arrCom[(UINT)type]);

		// 입력된 컴포넌트가 
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
		if (pRenderCom != nullptr)
		{
			// 이미 렌더링 컴포넌트를 가지고있는 경우
			assert(!m_RenderCom);
			m_RenderCom = pRenderCom;
		}

		m_arrCom[(UINT)type] = _Component;
	}
	_Component->m_Owner = this;
}

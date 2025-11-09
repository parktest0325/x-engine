#pragma once
#include "CEntity.h"

#include "CGameObject.h"

class CComponent :
    public CEntity
{
private:
    CGameObject*            m_Owner;    // 해당 컴포넌트를 소유하고 있는 오브젝트를 알아야함
    const COMPONENT_TYPE    m_Type;     // 본인이 어떤 컴포넌트인지 정보


public:
    virtual void begin() {};
    virtual void tick() {};
    virtual void finaltick() = 0;


public:
    COMPONENT_TYPE GetComponentType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }

public:
    CComponent(COMPONENT_TYPE _Type);
    virtual ~CComponent();

    friend class CGameObject;           // 게임오브젝트가 컴포넌트를 add할때 Owner를 기록할 수 있도록
};


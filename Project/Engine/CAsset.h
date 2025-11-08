#pragma once
#include "CEntity.h"
class CAsset :
    public CEntity
{
private:
    wstring             m_Key;          // 로딩됐을때 키값
    wstring             m_RelativePath; // 로딩된 에셋의 상대경로(폴더) 위치
    const ASSET_TYPE    m_Type;         // Asset의 타입

    int                 m_RefCount;     // 참조카운트

public:
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }
    const ASSET_TYPE GetAssetType() { return m_Type; }

private:
    void AddRef() { m_RefCount++; }
    void Release()
    {
        m_RefCount--;
        if (m_RefCount <= 0)
        {
            delete this;
        }
    }

public:
    CAsset(ASSET_TYPE _Type);
    CAsset(const CAsset& _Origin) = delete;
    ~CAsset();

	template<typename T>
    friend class Ptr;
};

#pragma once
#include "CEntity.h"
class CAsset :
    public CEntity
{
private:
    wstring             m_Key;          // 로딩됐을때 키값
    wstring             m_RelativePath; // 로딩된 에셋의 상대경로(폴더) 위치
    const ASSET_TYPE    m_Type;         // Asset의 타입

public:
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }
    const ASSET_TYPE GetAssetType() { return m_Type; }

public:
    CAsset(ASSET_TYPE _Type);
    ~CAsset();
};


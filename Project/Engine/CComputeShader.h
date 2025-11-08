#pragma once
#include "CShader.h"

// GPU 대량 연산처리할때 사용하는게 컴퓨트쉐이더
class CComputeShader :
    public CShader
{
public:
    CComputeShader();
    ~CComputeShader();
};


#ifndef _STD2D
#define _STD2D

cbuffer TRANSFORM : register(b0)	// 레지스터 번호 필요
{
	float4 g_Position;
}

struct VS_IN
{
	float3 vPos : POSITION0;		// SymenticName(+SymenticIndex)
};

struct VS_OUT
{
	float4 vPosition : SV_Position;
};

VS_OUT VS_Std2D(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vPos + g_Position.xyz, 1.f);

	return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
	return float4(0.2f, 0.2f, 0.5f, 1.f);
}

#endif
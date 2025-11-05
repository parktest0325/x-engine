#pragma once

// 정점
// 3D 공간의 좌표를 나타내는 단위
struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
};

// 상수버퍼 관련
struct tTransform
{
	Vec4 Position;	// x, y, z, padding
	Vec4 Scale;
};

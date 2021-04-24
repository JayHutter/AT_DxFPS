#pragma once
#include <DirectXMath.h>

class Billboard
{
public:
	Billboard(float* pitch, float* yaw);
	void FacePlayer(float playPitch, float playYaw);
	void FacePlayer(DirectX::XMFLOAT2 rot);
private:
	float* pitch;
	float* yaw;
};


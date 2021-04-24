#pragma once
#include <DirectXMath.h>
class Camera
{
public:
	void Update(float _x, float _y, float _z, float _roll, float _pitch, float _yaw);
	void Update(DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _rot);
	DirectX::XMMATRIX GetTransform();
	DirectX::XMMATRIX GetPosOnly();

private:
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
};


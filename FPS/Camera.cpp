#include "Camera.h"

void Camera::Update(float _x, float _y, float _z, float _roll, float _pitch, float _yaw)
{
	pos = { _x, _y, _z };
	rot = { _yaw, _pitch, _roll };
}

void Camera::Update(DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _rot)
{
	pos = _pos;
	rot = _rot;
}

DirectX::XMMATRIX Camera::GetTransform()
{
	return DirectX::XMMatrixTranslation(-pos.x, -pos.y, -pos.z) * //Player pos offset
		DirectX::XMMatrixRotationRollPitchYaw(0, -rot.x, 0) * //Horizontal rotation
		DirectX::XMMatrixRotationRollPitchYaw(-rot.y, 0, 0); //Vertical rotation
}

DirectX::XMMATRIX Camera::GetPosOnly()
{
	return DirectX::XMMatrixTranslation(-pos.x, -pos.y, -pos.z) * //Horizontal rotation
		DirectX::XMMatrixRotationRollPitchYaw(-rot.y, 0, 0);
}

#include "GameObject.h"

DirectX::XMFLOAT3 GameObject::GetPos() const noexcept
{
	return pos;
}

float GameObject::CameraDistance() const noexcept
{
	return distanceToCam;
}

bool GameObject::BlocksAi() const noexcept
{
	return blockAi;
}

GameObject::~GameObject()
{
	if (collider)
	{
		delete collider;
		collider = nullptr;
	}

	if (billboard)
	{
		delete billboard;
		billboard = nullptr;
	}
}

void GameObject::updateLifeTime(float dt) noexcept
{
	if (lifeTime < 0.0f)
	{
		return;
	}

	lifeTime -= dt;
	if (lifeTime <= 0.0f)
	{
		destroy = true;
	}
}

DirectX::XMMATRIX GameObject::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(rot.y, rot.x, rot.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

Collision* GameObject::GetCollider() const noexcept
{
	return collider;
}

Billboard* GameObject::GetBillboard() const noexcept
{
	return billboard;
}

bool GameObject::Destroy() noexcept
{
	return destroy;
}

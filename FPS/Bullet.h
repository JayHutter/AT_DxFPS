#pragma once
#include "Object.h"

class Bullet : public DrawableBase<Bullet>
{
public:
	Bullet(Renderer& renderer, float x, float y, float z, DirectX::XMFLOAT3 vel = { 0.0f, 0.0f, 0.0f });
	void Update(float dt) noexcept override;
	void TestCollision(GameObject* other, float dt) noexcept override;

private:
	DirectX::XMFLOAT3 velocity;
};


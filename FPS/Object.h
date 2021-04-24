#pragma once
#include "DrawableBase.h"
#include "Collision.h"
#include "Billboard.h"

template<class T>
class Object : public DrawableBase<T>
{
	friend class Drawable;
public:
	Object(float x, float y, float z,
		float yaw, float pitch, float roll,
		float width, float height, float depth):
		x(x), y(y), z(z),
		yaw(yaw), pitch(pitch), roll(roll),
		width(width), height(height), depth(depth)
	{
	}
	void Update(float dt) noexcept override {}
	DirectX::XMMATRIX GetTransformXM() const noexcept override
	{
		return DirectX::XMMatrixScaling(width, height, depth) *
			DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(x, y, z);
	}

	Collision* GetCollider() const noexcept override
	{
		return collider;
	}

	Billboard* GetBillboard() const noexcept override
	{
		return billboard;
	}

	void OnCollision(Drawable* other) noexcept override {}

	bool Destroy() noexcept override
	{
		return destroy;
	}

	~Object() = default;
protected:
	float x;
	float y;
	float z;

	float width = 1.0f;
	float height = 1.0f;
	float depth = 1.0f;

	float yaw;
	float pitch;
	float roll;
	
	Collision* collider = nullptr;
	Billboard* billboard = nullptr;
	bool destroy = false;
};
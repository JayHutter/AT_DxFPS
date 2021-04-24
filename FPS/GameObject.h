#pragma once
#include "Collision.h"
#include "Billboard.h"
#include "Renderer.h"
#include <DirectXMath.h>

class GameObject
{
public:
	GameObject() = default;

	virtual void Draw(Renderer& renderer) const noexcept {}
	virtual void Update(float dt) noexcept = 0;
	void updateLifeTime(float dt) noexcept;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept;
	virtual Collision* GetCollider() const noexcept;
	virtual Billboard* GetBillboard() const noexcept;
	virtual bool Destroy() noexcept;
	virtual void TestCollision(GameObject* other, float dt) noexcept {}

	DirectX::XMFLOAT3 GetPos() const noexcept;

	virtual void SetCameraDistance(float dist) noexcept {};
	float CameraDistance() const noexcept;
	bool BlocksAi()const noexcept;

	virtual ~GameObject();
protected:
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rot = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	Collision* collider = nullptr;
	Billboard* billboard = nullptr;
	bool destroy = false;

	float distanceToCam = 0.0f;
	float lifeTime = -1.0f;
	bool blockAi = false;
};


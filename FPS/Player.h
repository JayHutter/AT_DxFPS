#pragma once
#include <DirectXMath.h>
#include "GameObject.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Renderer.h"

class Player : public GameObject
{
public:
	Player(float gridScale, std::array<int, 3> startPos = { 0, 0, 0 });
	~Player();
	void LinkPointers(bool* completed, bool* lost);

	void Update(float dt) noexcept override;
	void TestCollision(GameObject* other, float dt) noexcept override;

	void Input(Keyboard& key, Mouse& mse, float dt, std::vector<std::unique_ptr<class GameObject>> &objects, Renderer& renderer);

	void Move(float horizontal, float vertical);
	void Rotate(float pitch, float yaw, float dt);

	DirectX::XMFLOAT3 Position();
	DirectX::XMFLOAT3 CameraAngle();

	DirectX::XMFLOAT3 ForwardVector();
	DirectX::XMFLOAT3 RightVector();

	void AddForce(float x, float y, float z);

	float DistanceFrom(float x, float y, float z);
	float DistanceFrom(DirectX::XMFLOAT3 coord);

private:
	void Friction();
	void NormaliseVel();
	float Speed();

	const float speed = 10.0f;
	const float rotspeed = 0.5f;
	float moveSpeed = 10.0f;

	DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };

	bool invert = true;

	float health = 100;

	float fireTimer = 0.0f;
	const float fireDelay = 0.2f;
	const float height = 1.7f;;
	bool* completed = nullptr;
	bool* death = nullptr;
};


#pragma once
#include <DirectXMath.h>
#include <array>
#include <string>
#include <functional>

class Collision
{
public:
	struct Collider
	{
		struct
		{
			float a;
			float b;
		} x;
		struct
		{
			float a;
			float b;
		} y;
		struct
		{
			float a;
			float b;
		} z;
	};

	Collision(float width, float height, float depth, float posX, float posY, float posZ, bool trigger, std::string tag);
	void UpdateTag(std::string newTag);

	void UpdatePos(float x, float y, float z);
	void Resize(float width, float height, float depth);
	void Enable(bool enable);
	void SwitchState();

	bool Colliding(Collision *other);
	bool Colliding(float x, float y, float z);

	bool Triggered(float x, float y, float z);

	DirectX::XMFLOAT3 GetResistiveForce();
	DirectX::XMFLOAT3 GetCentre();

	Collider* Hitbox();

	std::string Tag();

private:
	void CalculateResistance(float x, float y, float z);

	Collider bindingBox = { -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f };
	//std::array<float, 3> scale = { 0.5f, 0.5f, 0.5f };
	DirectX::XMFLOAT3 scale = { 0.5f, 0.5f, 0.5f };
	bool enabled = true;
	bool trigger = false;
	//std::array<float, 3> force = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 force = { 0.0f, 0.0f, 0.0f };
	std::string tag = "";
};


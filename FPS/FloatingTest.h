#pragma once
#include "Drawable.h"
#include "DrawableBase.h"

class FloatingTest : public DrawableBase<FloatingTest>
{
public:
	FloatingTest(Renderer& renderer, float x, float y, float z, float width, float height, float depth);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	//position
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	float x;
	float y;
	float z;

	float droll = 0.5f;
	float dpitch= 0.5f;
	float dyaw = 0.5f;
	float dtheta;
	float dphi;
	float dchi;

	float width = 1.0f;
	float height = 1.0f;
	float depth = 1.0f;

	float timer = 0;
	float speed = 1;

	float cameraangle = 0;
	float cameraspeed = 0.5f;
	//transform
	DirectX::XMFLOAT3X3 mt;
};


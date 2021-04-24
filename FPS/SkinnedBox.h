#pragma once
#include "DrawableBase.h"

class SkinnedBox : public DrawableBase<SkinnedBox>
{
public:
	SkinnedBox(Renderer& renderer, float x, float y, float z);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	float xPos = 0.0f;
	float yPos = 0.0f;
	float zPos = 0.0f;
};
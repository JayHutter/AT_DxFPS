#pragma once
#include "Drawable.h"
#include "Object.h"

class Box : public DrawableBase<Box>
{
public:
	Box(Renderer& renderer, float x, float y, float z);
	void Update(float dt) noexcept override;
private:
	//speed
	float droll;
	float dpitch;
	float dyaw;
};


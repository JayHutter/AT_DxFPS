#pragma once
#include "Drawable.h"
#include "Object.h"

class Shadow : public DrawableBase<Shadow>
{
public:
	Shadow(Renderer& renderer, float x, float y, float z, float width, float height, float pitch);
	void Update(float dt) noexcept override;
private:
};


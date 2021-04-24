#pragma once
#include "Drawable.h"
#include "Object.h"

class Wall : public DrawableBase<Wall>
{
public:
	Wall(Renderer& renderer, int x, int y, int flr);
	void Update(float dt) noexcept override;
private:
};


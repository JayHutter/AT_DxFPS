#pragma once
#include "Drawable.h"
#include "Object.h"

class Floor : public DrawableBase<Floor>
{
public:
	Floor(Renderer& renderer, int x, int y, int flr);
	void Update(float dt) noexcept override;
private:
};


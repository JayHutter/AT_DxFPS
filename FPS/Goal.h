#pragma once
#include "Drawable.h"
#include "Object.h"

class Goal : public DrawableBase<Goal>
{
public:
	Goal(Renderer& renderer, int x, int y, int flr);
	void Update(float dt) noexcept override;
private:
};


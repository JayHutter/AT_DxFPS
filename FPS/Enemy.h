#pragma once
#include "Drawable.h"
#include "Object.h"
#include "Shadow.h"

class Enemy : public DrawableBase<Enemy>
{
public:
	Enemy(Renderer& renderer, int x, int y, int flr);
	void Update(float dt) noexcept override;
	//void FacePlayer(float _pitch, float _yaw);
	virtual void TestCollision(GameObject* other) noexcept;
	void Damage(float dmg);
private:
	void ShootPlayer();
	float health = 100.0f;
	float iframe = 0.0f;

};


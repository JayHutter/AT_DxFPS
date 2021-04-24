#include "Collision.h"
#include <math.h>

Collision::Collision(float width, float height, float depth, float posX, float posY, float posZ, bool trigger, std::string tag) :
	trigger(trigger), tag(tag)
{
	Resize(width, height, depth);
	UpdatePos(posX, posY, posZ);
}

void Collision::UpdateTag(std::string newTag)
{
	tag = newTag;
}

void Collision::UpdatePos(float x, float y, float z)
{
	bindingBox.x.a = x - scale.x;
	bindingBox.x.b = x + scale.x;

	bindingBox.y.a = y - scale.y;
	bindingBox.y.b = y + scale.y;

	bindingBox.z.a = z - scale.z;
	bindingBox.z.b = z + scale.z;
}

void Collision::Resize(float width, float height, float depth)
{
	scale = { width / 2, height / 2, depth / 2 };
}

void Collision::Enable(bool enable)
{
	enabled = enable;
}

void Collision::SwitchState()
{
	enabled = !enabled;
}

bool Collision::Colliding(Collision *other)
{
	//Define two box colliders colliding
	if (!enabled)
	{
		return false;
	}

	if (bindingBox.x.a < other->Hitbox()->x.b && bindingBox.x.b > other->Hitbox()->x.a &&
		bindingBox.y.a < other->Hitbox()->y.b && bindingBox.y.b > other->Hitbox()->y.a &&
		bindingBox.z.a < other->Hitbox()->z.b && bindingBox.z.b > other->Hitbox()->z.a)
	{
		//Calculate interseticion point
		//CalculateResistance(intersection)

		DirectX::XMFLOAT3 otherCentre = other->GetCentre();
		CalculateResistance(otherCentre.x, otherCentre.y, otherCentre.z);

		return true;
	}

	return false;
}

//Testing player position is colliding (less accurate)
bool Collision::Colliding(float x, float y, float z)
{
	if (!enabled)
	{
		return false;
	}


	if ((x >= bindingBox.x.a && x <= bindingBox.x.b) &&
		(y >= bindingBox.y.a && y <= bindingBox.y.b) &&
		(z >= bindingBox.z.a && z <= bindingBox.z.b))
	{
		force = { 0.0f, 0.0f, 0.0f };
		if (!trigger)
		{
			CalculateResistance(x, y, z);
		}
		return true;
	}

	return false;
}

bool Collision::Triggered(float x, float y, float z)
{
	if (!enabled || !trigger)
	{
		return false;
	}


	if ((x >= bindingBox.x.a && x <= bindingBox.x.b) &&
		(y >= bindingBox.y.a && y <= bindingBox.y.b) &&
		(z >= bindingBox.z.a && z <= bindingBox.z.b))
	{
		return true;
	}

	return false;
}

DirectX::XMFLOAT3 Collision::GetResistiveForce()
{
	return force;
}

DirectX::XMFLOAT3 Collision::GetCentre()
{
	DirectX::XMFLOAT3 centre = { 0.0f, 0.0f, 0.0f };
	centre.x = bindingBox.x.a + scale.x;
	centre.y = bindingBox.y.a + scale.y;
	centre.z = bindingBox.z.a + scale.z;

	return centre;
}

Collision::Collider* Collision::Hitbox()
{
	return &bindingBox;
}

std::string Collision::Tag()
{
	return tag;
}


void Collision::CalculateResistance(float x, float y, float z)
{
	DirectX::XMFLOAT3 val = GetCentre();
	//Calculate Vector between centre and pos
	val.x = val.x - x;
	////val.y = y - val.y;
	val.y = 0;
	val.z = val.z - z;
	//
	float mag = sqrt((val.x * val.x) + (val.y * val.y) + (val.z * val.z));
	force.x = val.x;// / mag;
	force.y = val.y;// / mag;
	force.z = val.z;// / mag;
}
#include "Player.h"
#include <iostream>
#include "Bullet.h"

Player::Player(float gridScale, std::array<int, 3> startPos)
{

	pos.x = startPos[0] * gridScale;
	pos.z = startPos[1] * gridScale;
	pos.y = startPos[2] * gridScale + height;

	collider = new Collision(1.5f, 3, 1.5f, pos.x, pos.y, pos.z, false, "Player");
}

Player::~Player()
{
	if (completed)
	{
		completed = nullptr;
	}
}

void Player::LinkPointers(bool* completed, bool* lost)
{
	this->completed = completed;
	this->death = lost;
}

void Player::Update(float dt) noexcept
{
	pos.x += velocity.x * dt;
	pos.z += velocity.z * dt;

	collider->UpdatePos(pos.x, pos.y, pos.z);

	if (health <= 0)
	{
		*death = true;
	}

	Friction();
}

void Player::TestCollision(GameObject* other, float dt) noexcept
{
	Collision* otherCol = other->GetCollider();
	if (!otherCol)
	{
		return;
	}

	if (collider->Colliding(otherCol))
	{
		if (otherCol->Tag() == "World")
		{
			auto force = collider->GetResistiveForce(); 
			float mag = Speed();
			AddForce(force.x * mag, 0.0f, force.z * mag);
		}

		if (otherCol->Tag() == "Enemy")
		{
			auto force = collider->GetResistiveForce();
			float mag = Speed();
			AddForce(force.x * mag, 0.0f, force.z * mag);
			health -= 25;
		}
		if (otherCol->Tag() == "Goal")
		{
			*completed = true;
		}
	}
}

void Player::Input(Keyboard& key, Mouse& mse, float dt, std::vector<std::unique_ptr<class GameObject>>& objects, Renderer& renderer)
{
	float vertical = key.KeyIsPressed('W') - key.KeyIsPressed('S');
	float horizontal = key.KeyIsPressed('D') - key.KeyIsPressed('A');

	moveSpeed = speed;
	if (key.KeyIsPressed(VK_LSHIFT) || key.KeyIsPressed(VK_SPACE))
	{
		moveSpeed *= 2;
	}

	Move(horizontal, vertical);

	float yaw = key.KeyIsPressed(VK_RIGHT) - key.KeyIsPressed(VK_LEFT);
	float pitch = key.KeyIsPressed(VK_UP) - key.KeyIsPressed(VK_DOWN);

	yaw = mse.GetMouseMoveX();
	pitch = mse.GetMouseMoveY() * (1 - 2 * invert);

	Rotate(pitch, yaw, dt);

	fireTimer += dt;

	if (mse.LeftIsPressed() && fireTimer >= fireDelay)
	{
		DirectX::XMFLOAT3 vel = { ForwardVector().x * speed * 2, ForwardVector().y * speed * 2, ForwardVector().z * speed * 2 };
		objects.push_back(std::make_unique<Bullet>(renderer, pos.x, pos.y, pos.z, vel));
		fireTimer = 0.0f;
	}
}

void Player::Move(float horizontal, float vertical)
{
	if (horizontal == 0.0f && vertical == 0.0f)
	{
		return;
	}

	DirectX::XMFLOAT3 forward = ForwardVector();
	DirectX::XMFLOAT3 right = RightVector();
	velocity.x += ((forward.x * vertical) + (right.x * horizontal));
	velocity.z += ((forward.z * vertical) + (right.z * horizontal));

	float mag = Speed();
	velocity.x = (velocity.x / mag) * moveSpeed;
	velocity.z = (velocity.z / mag) * moveSpeed;
}

void Player::Rotate(float pitch, float yaw, float dt)
{
	rot.x += yaw * dt * rotspeed;
	rot.y += pitch * dt * rotspeed;
	if (rot.y > DirectX::XM_PIDIV2)
	{
		rot.y = DirectX::XM_PIDIV2;
	}

	if (rot.y < -DirectX::XM_PIDIV4)
	{
		rot.y = -DirectX::XM_PIDIV4;
	}
}

DirectX::XMFLOAT3 Player::Position()
{
	return pos;
}

DirectX::XMFLOAT3 Player::CameraAngle()
{
	return rot;
}

DirectX::XMFLOAT3 Player::ForwardVector()
{
	DirectX::XMFLOAT3 forward;
	forward.x = sin(rot.x);
	forward.y = sin(-rot.y);
	forward.z = cos(rot.x);

	return forward;
}

DirectX::XMFLOAT3 Player::RightVector()
{
	DirectX::XMFLOAT3 right;
	right.x = cos(rot.x);
	right.y = sin(-rot.y);
	right.z = -sin(rot.x);

	return right;
}

void Player::AddForce(float x, float y, float z)
{
	velocity.x += x;
	//velocity.y += y;
	velocity.z += z;

}

float Player::DistanceFrom(float x, float y, float z)
{
	float dx = pos.x - x;
	float dy = pos.y - y;
	float dz = pos.z - z;

	float mag = sqrt((dx * dx) + (dy * dy) + (dz * dz));
	if (mag < 0) mag *= -1;

	return mag;
}

float Player::DistanceFrom(DirectX::XMFLOAT3 coord)
{
	float dx = pos.x - coord.x;
	float dy = pos.y - coord.y;
	float dz = pos.z - coord.z;

	float mag = sqrt((dx * dx) + (dy * dy) + (dz * dz));
	if (mag < 0) mag *= -1;

	return mag;
}

void Player::Friction()
{
	velocity.x *= 0.5f;
	velocity.z *= 0.5f;
}

void Player::NormaliseVel()
{
	float mag = Speed();
	velocity.x = velocity.x / mag;
	velocity.y = velocity.y / mag;
	velocity.z = velocity.z / mag;
}

float Player::Speed()
{
	return sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.z * velocity.z));
}

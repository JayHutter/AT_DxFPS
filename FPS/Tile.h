#pragma once
#include "GameObject.h"

class Tile
{
public:
	Tile() = default;
	void AttachObject(std::unique_ptr<GameObject>& obj);
	void RemoveObject(std::unique_ptr<GameObject> obj);

	bool Traversable();
private:
	std::vector<std::unique_ptr<GameObject>> objects;
	DirectX::XMFLOAT2 pos = { 0.0f, 0.0f };

};


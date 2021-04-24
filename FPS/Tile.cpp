#include "Tile.h"

bool BlockAi(const std::unique_ptr<GameObject>& obj)
{
	return (obj->BlocksAi());
}

void Tile::AttachObject(std::unique_ptr<GameObject>& obj)
{
	//objects.push_back(obj);
}

void Tile::RemoveObject(std::unique_ptr<GameObject> obj)
{
	//objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

bool Tile::Traversable()
{
	//auto it = std::find_if(objects.begin(), objects.end(), BlockAi);

	return true;
}

#include "Scene.h"

//Objects
#include "Player.h"
#include "Wall.h"
#include "Floor.h"
#include "Enemy.h"
#include "Goal.h"
#include "GameObject.h"
#include "Bullet.h"

#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

//Func for sorting the object vector
bool FurthestFirst(const std::unique_ptr<GameObject>& objA, const std::unique_ptr<GameObject>& objB)
{
	return objA->CameraDistance() > objB->CameraDistance();
}

Scene::Scene(Window& win) : win(win)
{
	//objects.push_back(std::make_unique<Player>());
	//player = dynamic_cast<Player*>(objects[0].get());
	//
	//EmptyTestLevel();
	//
	////int testLevel = 0;
	////switch (testLevel)
	////{
	////case 0:
	////	CreateTestLevel();
	////	break;
	////case 1:
	////	StressTestLevel();
	////	break;
	////case 2:
	////	EmptyTestLevel();
	////	break;
	////}
	//
	//lights.push_back(std::make_unique<PointLight>(win.Render(), 0, 10, 10));
	//win.Render().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, float(Cull::RENDER) - 10));
}

Scene::Scene(Window& win, std::string lvl) : win(win)
{
	if (!LoadLevel(lvl))
	{
		PostQuitMessage(-2); //TODO: Go back to start menu when made
	}

	//EmptyTestLevel();

	//lights.push_back(std::make_unique<PointLight>(win.Render(), 0, 10, 10)); //While multiple lights doesnt work - TO MOVE TO LEVEL LOADER
	win.Render().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, float(Cull::RENDER) - 10));
}

Scene::~Scene()
{
	if (player)
	{
		player = nullptr;
	}

	objects.clear();
}

void Scene::Update(float dt)
{
	if (play)
	{
		player->Input(win.key, win.mouse, dt, objects, win.Render());
		win.Render().ClearBuffer(bgCol[0], bgCol[1], bgCol[2]);

		LightScene();
		ManageObjects(dt);
		RenderScene();
	}
}

bool Scene::Completed()
{
	return completed;
}

bool Scene::Lost()
{
	return lost;
}

void Scene::EmptyTestLevel()
{
	objects.push_back(std::make_unique<Player>(win.Render().GridScale(), std::array<int, 3>{20, 20, 0}));
	player = dynamic_cast<Player*>(objects[0].get());

	float size = 20;

	for (int x = 0; x <= size; x += 1)
	{
		for (int y = 0; y <= size; y += 1)
		{
			if (x == size || x == 0 || y == size || y == 0)
			{
				objects.push_back(std::make_unique<Wall>(win.Render(), x, y, 0));
			}
			if ((x % 2 == 0 && y % 2 == 0))
			{
				objects.push_back(std::make_unique<Enemy>(win.Render(), x, y, 0));
			}

			objects.push_back(std::make_unique<Floor>(win.Render(), x, y, 0));
		}
	}
}

bool Scene::LoadLevel(std::string name)
{
	std::ifstream file("levels/" + name + ".json");
	if (!file.is_open())
	{
		return false;
	}

	json level;
	file >> level;
	file.close();
	
	bgCol = level["skybox"].get<std::array<float, 3>>();
	auto world = level["world"];
	for (int f = 0; f < world.size(); f++) //floor level
	{
		for (int y = 0; y < world[f].size(); y++)
		{
			for (int x = 0; x < world[f][y].size(); x++)
			{
				auto obj = world[f][y][x].get<World>();

				switch (obj)
				{
				case World::EMPTY:
					break;
				case World::EXIT:
				{
					objects.push_back(std::make_unique<Goal>(win.Render(), x, y, f));
				}
				case World::FLOOR:
					objects.push_back(std::make_unique<Floor>(win.Render(), x, y, f));
					break;
				case World::WALL:
					objects.push_back(std::make_unique<Wall>(win.Render(), x, y, f));
					break;
				}
			}
		}
	}

	auto enemies = level["enemies"];
	for (auto& e : enemies)
	{
		auto pos = e.get<std::array<int, 3>>();
		objects.push_back(std::make_unique<Enemy>(win.Render(), e[0], e[1], e[2]));
	}

	auto startPos = level["startpos"].get<std::array<int, 3>>();
	bool* test = nullptr;
	objects.push_back(std::make_unique<Player>(win.Render().GridScale(), startPos));
	player = dynamic_cast<Player*>(objects.back().get());
	player->LinkPointers(&completed, &lost);

	auto lightPositions = level["lights"];
	for (auto& l : lightPositions)
	{
		lights.push_back(std::make_unique<PointLight>(win.Render(), l[0], l[1], l[2], l[3]));
	}

	return true;
}

//Sort furthest to closest so that the transparency works
void Scene::SortVector()
{
	std::sort(objects.begin(), objects.end(), FurthestFirst);
}

void Scene::RenderScene()
{
	SortVector();
	win.Render().GetCamera()->Update(player->Position(), player->CameraAngle());
	win.Render().DrawFrame();
}

void Scene::ManageCollisions(GameObject* obj, float dt)
{
	if (!obj->GetCollider())
	{
		return;
	}

	for (auto& other : objects)
	{
		if (obj == other.get())
		{
			continue;
		}
		if (!other->GetCollider())
		{
			continue;
		}
		//Skip objects too far from player
		if (player->DistanceFrom(other->GetPos()) > float(Cull::COLLISION))
		{
			continue;
		}

		obj->TestCollision(other.get(), dt);
	}
}

void Scene::ManageObjects(float dt)
{
	float furthestDist = objects[0]->CameraDistance();

	for (auto& obj : objects)
	{
		if (!obj)
		{
			continue;
		}

		obj->updateLifeTime(dt);

		obj->SetCameraDistance(player->DistanceFrom(obj->GetPos()));

		Billboard* bboard = obj->GetBillboard();
		if (obj->CameraDistance() < float(Cull::UPDATE) && bboard)
		{
			bboard->FacePlayer(player->CameraAngle().y, player->CameraAngle().x);
			obj->Draw(win.Render());
		}

		if (obj->CameraDistance() < float(Cull::RENDER) && !bboard)
		{
			obj->Draw(win.Render());
		}
		

		if (obj->CameraDistance() < float(Cull::UPDATE))
		{
			obj->Update(dt);
		}		
		
		if (obj->CameraDistance() < float(Cull::COLLISION))
		{
			ManageCollisions(obj.get(), dt);
		}

		if (obj->Destroy())
		{
			objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
		}
	}
}

void Scene::LightScene()
{
	for (auto& l : lights)
	{
		l->Bind(win.Render(), win.Render().GetCamera()->GetTransform());
	}
}

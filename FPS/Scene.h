#pragma once
#include "Window.h"
#include "GameObject.h"
#include "PointLight.h"
#include "Player.h"

class Scene
{
public:
	Scene(Window& win);
	Scene(Window& win, std::string lvl);
	~Scene();

	void Update(float dt);
	bool Completed();
	bool Lost();

private:
	void CreateTestLevel();
	void StressTestLevel();
	void EmptyTestLevel();

	bool LoadLevel(std::string name);

	void SortVector();

	void RenderScene();
	void ManageCollisions(GameObject* obj, float dt);
	void ManageObjects(float dt);
	void LightScene();

	bool completed = false;
	bool lost = false;

	Window& win;

	std::vector<std::unique_ptr<class GameObject>> objects;
	std::vector<std::unique_ptr<class PointLight>> lights;
	std::vector<std::vector<std::unique_ptr<class GameObject>>> worldGrid;
	Player* player = nullptr;

	std::array<float, 3> bgCol = { 0.0f, 0.0f, 0.0f };

	bool play = true;

	enum class Cull
	{
		RENDER = 100,
		UPDATE = 30,
		COLLISION = 20
	};

	enum class World
	{
		EMPTY = 0,
		FLOOR,
		WALL,
		EXIT
	};
};


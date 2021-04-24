#pragma once
#include "Window.h"
#include "Timer.h"
#include "PointLight.h"
#include "Drawable.h"
#include "PointLight.h"
#include "GameObject.h"
#include "Scene.h"

class Game
{
public:
	Game();
	~Game() = default;

	int Run();
private:
	void Update();
	void ManageScene(float dt);
	void LoadLevel(int level);
	void WinScreen(float dt);

	Window win;
	Timer timer;

	Scene* scene = nullptr;
	int currentLevel = 0;

	bool completed = false;
};

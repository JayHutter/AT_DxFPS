#include "Game.h"
#include "GDIPlusManager.h"

#include <DirectXMath.h>

#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

GDIPlusManager gdipm;

Game::Game() : win(1280, 720, "J AT FPS")
{
	win.Render().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));
	LoadLevel(0);
}

int Game::Run()
{
	while (true)
	{
		if (const auto exit = Window::ManageMsgs())
		{
			return *exit;
		}
		if (win.hasFocus())
		{
			Update();
		}
	}
}

void Game::Update()
{
	auto dt = timer.DeltaTime();

	if (win.key.KeyIsPressed(VK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	ManageScene(dt);
}

void Game::ManageScene(float dt)
{
	//Scene* currentScene = scenes[currentLevel].get();

	if (completed)
	{
		WinScreen(dt);
	}

	scene->Update(dt);

	//Load next level on goal
	if (scene->Completed())
	{
		LoadLevel(++currentLevel);
	}

	//Reload on death
	if (scene->Lost())
	{
		LoadLevel(currentLevel);
	}
}

void Game::LoadLevel(int level)
{
	std::ifstream file("levels/LevelNames.json");
	if (!file.is_open())
	{
		PostQuitMessage(-10);
	}

	json levelNames;
	file >> levelNames;
	file.close();

	auto names = levelNames["names"];

	if (level >= names.size())
	{
		completed = true;
		delete scene;
		scene = new Scene(win, "WinScreen");
		return;
	}

	std::string name = names[level];

	delete scene;
	scene = new Scene(win, name);
}

void Game::WinScreen(float dt)
{
	if (win.key.KeyIsPressed(VK_SPACE))
	{
		currentLevel = 0;
		LoadLevel(0);
		completed = false;
	}
}

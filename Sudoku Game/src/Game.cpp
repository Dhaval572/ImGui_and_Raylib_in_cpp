#include "Game.h"
#include <rlImGui.h>
#include <imgui.h>
#include <raylib.h>
#include "ImGuiCustomTheme.h"

Game::Game() : running(true) {}
Game::~Game() { if (running) Shutdown(); }

void Game::InitRaylib()
{
	InitWindow(1200, 700, "Solve Sudoku");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	MaximizeWindow();
	rlImGuiSetup(true);
	ImCustomTheme();
	SetTargetFPS(60);
}

void Game::CloseRaylib()
{
	rlImGuiShutdown();
	CloseWindow();
}

bool Game::Initialize()
{
	InitRaylib();
	return true;
}

void Game::Shutdown()
{
	if (running)
	{
		CloseRaylib();
		running = false;
	}
}

void Game::Update()
{
	// Game logic updates go here
}

void Game::Render()
{
	BeginDrawing();
	ClearBackground(Color{BLACK});
	rlImGuiBegin();

	ImGui::SetNextWindowPos(ImVec2(350, 20));
	ImGui::SetNextWindowSize
	(
		ImVec2
		(
			GetScreenWidth() - 720, 
			GetScreenHeight() - 40
		)
	);
	
	ImGui::Begin("Sudoku", nullptr, ImGuiWindowFlags_NoResize);
	renderer.RenderGrid(board);
	renderer.RenderControls(board);
	ImGui::End();

	rlImGuiEnd();
	EndDrawing();
}

void Game::Run()
{
	if (!Initialize()) return;

	while (!WindowShouldClose() && running)
	{
		Update();
		Render();
	}

	Shutdown();
}
#pragma once
#include "SudokuBoard.h"
#include "SudokuRenderer.h"

class Game
{
private:
	SudokuBoard board;
	SudokuRenderer renderer;
	bool running;

	void InitRaylib();
	void CloseRaylib();
	void Update();
	void Render();

public:
	Game();
	~Game();

	bool Initialize();
	void Shutdown();
	void Run();
};
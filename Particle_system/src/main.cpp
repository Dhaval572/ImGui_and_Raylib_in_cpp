#include <rlImGui.h>
#include <imgui.h>
#include "Particle_system.h"
#include "ImGuiCustomTheme.h"

int main()
{
	InitWindow(800, 600, "raylib + ImGui Example");

	SetTargetFPS(60);
	rlImGuiSetup(true);
	ImCustomTheme();
 
	ParticleSystem ps({100, 100}, 10, true, BLUE);

	while (!WindowShouldClose())
	{
		ps.Update(GetFrameTime());
		BeginDrawing();
		ClearBackground(DARKGRAY);
		rlImGuiBegin();
		ps.Draw();
		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
	return 0;
}
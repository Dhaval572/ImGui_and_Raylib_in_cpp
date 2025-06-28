#include <rlImGui.h>
#include "ImGuiCustomTheme.h"
#include "ParticleSystem.h"

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1200, 700, "Particle System Demo");
	SetTargetFPS(60);
	rlImGuiSetup(true);
	ImCustomTheme();

	ParticleSystem particleSystem;
	bool showEmitterShape = true;

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		particleSystem.Update(deltaTime);

		particleSystem.position = {100, 350};

		BeginDrawing();
		ClearBackground(BLACK);

		particleSystem.Draw();
		if (showEmitterShape)
		{
			particleSystem.DrawEmitterShape();
		}
		rlImGuiBegin();
		DrawParticleSystemUI(particleSystem);
		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();

	return 0;
}
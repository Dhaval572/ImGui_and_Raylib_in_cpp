// Funny code
#include <rlImGui.h>
#include <imgui.h>
#include <algorithm>
#include "ImGuiCustomTheme.h"

int main()
{
	InitWindow(800, 600, "raylib + ImGui Docking Example");
	
	static bool yes = false;
	static ImVec2 noButtonPos = ImVec2(400, 300); 
	
	rlImGuiSetup(true);
	// ImCustomTheme();
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(DARKGRAY);
		rlImGuiBegin();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Always);
		ImGui::Begin("Debug");

		ImGui::TextWrapped("Do you love me? ");
		if (ImGui::Button("Yes"))
		{
			yes = true;
		}


		ImVec2 mousePos = ImGui::GetMousePos();

		float dx = mousePos.x - noButtonPos.x;
		float dy = mousePos.y - noButtonPos.y;
		float dist = sqrtf(dx * dx + dy * dy);

		if (dist < 75.0f)
		{
			ImVec2 dir = ImVec2(noButtonPos.x - mousePos.x, noButtonPos.y - mousePos.y);
			float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
			if (len > 0.0f)
			{
				dir.x /= len;
				dir.y /= len;

				// Move speed
				noButtonPos.x += dir.x * 10.0f; 
				noButtonPos.y += dir.y * 10.0f;

				noButtonPos.x = std::clamp(noButtonPos.x, -100.0f, GetScreenWidth() - 100.0f);
                noButtonPos.y = std::clamp(noButtonPos.y, -100.0f, GetScreenHeight() - 30.0f);
			}
		}

		ImGui::SetCursorScreenPos(noButtonPos);
		ImGui::Button("No"); 
		if (yes)
		{
			ImGui::SetCursorScreenPos(ImVec2(400, 300));
			ImGui::TextWrapped("I love you too, my pretty little baby.");
		}

		ImGui::End();
		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
	return 0;
}

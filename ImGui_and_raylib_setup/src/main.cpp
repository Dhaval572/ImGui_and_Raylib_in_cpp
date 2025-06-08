// Demo code
#include <rlImGui.h>
#include <imgui.h>
#include "ImGuiCustomTheme.h"
#include "tinyfiledialogs.h"

int main()
{
	InitWindow(800, 600, "raylib + ImGui Example");

	SetTargetFPS(60);
	rlImGuiSetup(true);
	ImCustomTheme();

	char selectedFile[1024] = "";
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(DARKGRAY);
		rlImGuiBegin();

		if (ImGui::Button("Open File Dialog"))
		{
			const char *filterPatterns[] = {"*.png", "*.jpg", "*.jpeg"};
			const char *file = tinyfd_openFileDialog(
				"Select an Image",
				"",
				3,
				filterPatterns,
				"Image files",
				0);

			if (file)
			{
				strncpy(selectedFile, file, sizeof(selectedFile));
				selectedFile[sizeof(selectedFile) - 1] = '\0';
			}
		}

		if (selectedFile[0] != '\0')
		{
			ImGui::Text("Selected File: %s", selectedFile);
		}

		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
	return 0;
}
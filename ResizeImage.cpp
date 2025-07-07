// Resize image when rendering
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include "ImGuiCustomTheme.h"

int main()
{
    InitWindow(1000, 700, "raylib + ImGui - Runtime Image Resize");
    SetTargetFPS(60);
    rlImGuiSetup(true);
    ImCustomTheme();

    Image originalImage = LoadImage("assets/Xyz.png");
    Texture2D originalTexture = LoadTextureFromImage(originalImage);

    // Variables for resizing
    int resizeWidth = 200;
    int resizeHeight = 200;
    Texture2D resizedTexture;
    bool resizedTextureValid = false;

    bool showOriginal = true;
    bool needResize = true;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        rlImGuiBegin();

        ImGui::Begin("Image Resize Visualization");

        ImGui::Checkbox("Show Original Image", &showOriginal);
        ImGui::Text
		(
			"Original Texture Size: %d x %d", 
			originalTexture.width, 
			originalTexture.height
		);

        // Controls to adjust resize dimensions
        if (ImGui::SliderInt("Resize Width", &resizeWidth, 10, originalTexture.width))
		{
            needResize = true;
		}

        if (ImGui::SliderInt("Resize Height", &resizeHeight, 10, originalTexture.height))
		{
			needResize = true;
		}
		// Re-resize the image if dimensions changed
        if (needResize)
        {
            if (resizedTextureValid) 
				UnloadTexture(resizedTexture);

            Image resizedImage = ImageCopy(originalImage);
            ImageResize(&resizedImage, resizeWidth, resizeHeight);
            resizedTexture = LoadTextureFromImage(resizedImage);
            UnloadImage(resizedImage);

            resizedTextureValid = true;
            needResize = false;
        }

        if (showOriginal)
        {
            ImGui::Text("Displaying Original (scaled):");
            ImGui::Image
			(
				(ImTextureID)(uintptr_t)originalTexture.id, 
				ImVec2
				(
					originalTexture.width / 2.0f, originalTexture.height / 2.0f
				)
			);
        }
        else if (resizedTextureValid)
        {
            ImGui::Text("Displaying Resized Image:");
            ImGui::Image
			(
				(ImTextureID)(uintptr_t)resizedTexture.id, 
				ImVec2
				(
					(float)resizeWidth, (float)resizeHeight
				)
			);
            ImGui::Text("Resized Texture Size: %d x %d", resizeWidth, resizeHeight);
        }

        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    if (resizedTextureValid) 
		UnloadTexture(resizedTexture);

    UnloadTexture(originalTexture);
    UnloadImage(originalImage);

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

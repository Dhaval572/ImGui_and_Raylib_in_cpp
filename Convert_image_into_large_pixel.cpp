// View image in large pixel
#include <rlImGui.h>
#include <imgui.h>
#include <stdint.h>
#include "tinyfiledialogs.h"
#include "ImGuiCustomTheme.h"
constexpr uint8_t SCALE = 10;

const char *takeImageFromPc()
{
	const char *filters[4] = {"*.jpg", "*.jpeg", "*.png", "*.bmp"};
	return tinyfd_openFileDialog("Select an Image", "", 4, filters, "Image files", 0);
}

int main()
{
	InitWindow(800, 600, "Image Pixel Viewer");

	const char *filePath = nullptr;
	Image image = {0};
	Color *pixels = nullptr;
	bool imageLoaded = false;

	SetTargetFPS(60);
	rlImGuiSetup(true);
	ImCustomTheme();

	while (!WindowShouldClose())
	{
		int offsetX = (GetScreenWidth() - image.width * SCALE) / 2;
		int offsetY = (GetScreenHeight() - image.height * SCALE) / 2;

		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (imageLoaded)
		{
			// Draw each pixel as a rectangle
			for (uint8_t y = 0; y < image.height; y++)
			{
				for (uint8_t x = 0; x < image.width; x++)
				{
					Color color = pixels[y * image.width + x];
					DrawRectangle(offsetX + x * SCALE, offsetY + y * SCALE, SCALE, SCALE, color);
				}
			}
		}
		else
		{
			DrawText("Click 'Load' to select an image (max 100x100)", 10, 10, 20, GRAY);
		}

		rlImGuiBegin();

		if (ImGui::Button("Load"))
		{
			const char *newPath = takeImageFromPc();

			if (newPath)
			{
				Image newImage = LoadImage(newPath);

				if (newImage.width <= 100 && newImage.height <= 100)
				{
					// Free previous resources
					if (imageLoaded)
					{
						UnloadImageColors(pixels);
						UnloadImage(image);
					}

					ImageFormat(&newImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
					image = newImage;
					pixels = LoadImageColors(image);
					filePath = newPath;
					imageLoaded = true;
				}
				else
				{
					UnloadImage(newImage);
					tinyfd_messageBox("Error", "Image is larger than 100x100 pixels.", "ok", "error", 1);
				}
			}
		}

		rlImGuiEnd();
		EndDrawing();
	}

	if (imageLoaded)
	{
		UnloadImageColors(pixels);
		UnloadImage(image);
	}
	CloseWindow();

	return 0;
}

#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>

int main()
{
    InitWindow(800, 600, "raylib + ImGui Example");
    SetTargetFPS(60);

    const char *path = "../assets/Mahayami.png";
    Texture2D texture = LoadTexture(path);
    bool isLoaded = false;

    const Vector2 buttonPos = {10, 10};
    const float buttonWidth = 120.0f;

    Rectangle sourceRec = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    Rectangle destRec = {
        GetScreenWidth() / 2.0f - 250.0f,
        GetScreenHeight() / 2.0f - 250.0f,
        500.0f,
        500.0f};

    rlImGuiSetup(true); 

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin(); 

        ImGui::SetNextWindowPos(ImVec2(buttonPos.x, buttonPos.y));
        ImGui::SetNextWindowSize({buttonWidth, 0}); 

        ImGui::Begin("Texture Controls", nullptr,
                     ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoCollapse);

        if (isLoaded)
        {
            if (ImGui::Button("Unload Texture", {buttonWidth - 20, 0}))
            {
                UnloadTexture(texture);
                isLoaded = false;
            }
        }
        else
        {
            if (ImGui::Button("Load Texture", {buttonWidth - 20, 0}))
            {
                texture = LoadTexture(path);
                isLoaded = true;
                sourceRec = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
            }
        }

        ImGui::End();

        if (isLoaded)
            DrawTexturePro(texture, sourceRec, destRec, {0, 0}, 0.0f, RAYWHITE);

        rlImGuiEnd(); 
        EndDrawing();
    }
}
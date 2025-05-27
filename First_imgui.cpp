#include <rlImGui.h>
#include <imgui.h>

int main()
{
    InitWindow(800, 600, "raylib + ImGui Example");
    
    SetTargetFPS(60);
    rlImGuiSetup(true);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        rlImGuiBegin();
        
        static short counter = 0;
        if (ImGui::Button("Click me"))
        {
            counter++;
        }
        ImGui::Text("You clicked %d times", counter);

        rlImGuiEnd();
        EndDrawing();
    }
    
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

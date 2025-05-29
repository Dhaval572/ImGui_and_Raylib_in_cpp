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
        
        // Wrapped text 
        ImGui::TextWrapped("This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! This is a Wrapped text! ");
        
        ImGui::NewLine();

        // Normal text
        ImGui::Text("This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text This is a normal text ");

        rlImGuiEnd();
        EndDrawing();
    }
    
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

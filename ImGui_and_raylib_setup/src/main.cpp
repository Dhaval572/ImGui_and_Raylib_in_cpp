// Very simplest calculator Demo
#include <rlImGui.h>
#include <imgui.h>
#include "ImGuiCustomTheme.h"

int main()
{
    InitWindow(800, 600, "raylib + ImGui Demo");

    SetTargetFPS(60);
    rlImGuiSetup(true);
    ImCustomTheme();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();

        static double num1 = 0;
        static double num2 = 0;
        static double result = 0;

        ImGui::Begin("Calculator");

        ImGui::InputDouble("Number 1", &num1);
        ImGui::InputDouble("Number 2", &num2);

        if (ImGui::Button("Add"))
            result = num1 + num2;

        if (ImGui::Button("Substract"))
            result = num1 - num2;

        if (ImGui::Button("Divide"))
            result = num1 / num2;

        if (ImGui::Button("Multiply"))
            result = num1 * num2;

        ImGui::Text("Result: %f", result);

        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
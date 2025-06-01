// Very simplest calculator
#include <rlImGui.h>
#include <imgui.h>

int main()
{
    InitWindow(800, 600, "raylib + ImGui Example");

    SetTargetFPS(60);
    rlImGuiSetup(true);

    // Font
    ImGuiIO &io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("assets/Fonts/Roboto-Regular.ttf", 20.0f);
	rlImGuiReloadFonts();

    // Theme
    ImGuiStyle &style = ImGui::GetStyle();
	style.Colors[ImGuiCol_FrameBg] = ImColor(0.22f, 0.22f, 0.22f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(0.2f, 0.2f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(0.3f, 0.3f, 0.3f, 1.0f);
	style.WindowRounding = 5.0f;

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
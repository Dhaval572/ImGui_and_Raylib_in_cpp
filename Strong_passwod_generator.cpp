// Strong Password generator of given length
#include <rlImGui.h>
#include <imgui.h>
#include <string>
#include <random>

const std::string CHARSET =
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789"
    "!@#$%^&*()-_=+[]{};:,.<>?/|";

std::string GenerateStrongPassword(int length)
{
    std::random_device rd;
    std::mt19937 generate(rd());
    ;
    std::uniform_int_distribution<> dist(0, CHARSET.size() - 1);

    std::string password;

    for (size_t i = 0; i < length; ++i)
    {
        password += CHARSET[dist(generate)];
    }
    return password;
}

int main()
{
    InitWindow(800, 600, "raylib + ImGui Example");

    int pass_len = 0;
    std::string generated_pass = "";

    SetTargetFPS(60);
    rlImGuiSetup(true);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        rlImGuiBegin();

        ImGui::SetNextWindowSize(ImVec2(200, 50), ImGuiWindowFlags_NoResize);
        ImGui::InputInt("Password Length", &pass_len);

        // Clamp the input
        if (pass_len < 4)
            pass_len = 4;
        if (pass_len > 16)
            pass_len = 16;

        if (ImGui::Button("Generate"))
        {
            generated_pass = GenerateStrongPassword(pass_len);
        }

        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Always);
        ImGui::Begin("Box", nullptr, ImGuiWindowFlags_NoResize);
        ImGui::TextWrapped("Generated Password is: %s", generated_pass.c_str());
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

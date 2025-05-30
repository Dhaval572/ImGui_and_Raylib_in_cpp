// Wave visulization in Imgui
#include <rlImGui.h>
#include <imgui.h>
#include <raymath.h>
#include <string>
#include <cstdint>
using namespace std;

// optimized way using uint8_t
enum WaveType : uint8_t
{
    SINE,
    COSINE,
    TANGENT,
    SQUARE,
    SAWTOOTH,
    WAVE_COUNT
};

const char *waveNames[] = {"Sine", "Cosine", "Tangent", "Square", "Sawtooth"};

float GetWaveValue(WaveType type, float x)
{
    switch (type)
    {
    case SINE:
        return sin(x);
    case COSINE:
        return cos(x);
    case TANGENT:
        return tan(x);
    case SQUARE:
        return (sin(x) > 0.0f) ? 1.0f : -1.0f;
    case SAWTOOTH:
        return 2.0f * (x / (2 * PI) - floor(x / (2 * PI) + 0.5f));
    default:
        return 0.0f;
    }
}

int main()
{
    InitWindow(800, 600, "Raylib + ImGui - Wave Visualizer");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    WaveType selectedWave = SINE;
    float frequency = 3.0f;
    float amplitude = 100.0f;
    float speed = 1.0f;
    float time = 0.0f;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw wave
        const int screenMidY = GetScreenHeight() / 2;
        const int screenWidth = GetScreenWidth();
        const float step = 1.0f;

        Vector2 prevPoint = {0, static_cast<float>(screenMidY)};

        for (float x = 0; x < screenWidth; x += step)
        {
            float worldX = (x + time) * frequency * 0.01f;
            float y = GetWaveValue(selectedWave, worldX);
            Vector2 point = {x, screenMidY - y * amplitude};
            DrawLineV(prevPoint, point, GREEN);
            prevPoint = point;
        }

        rlImGuiBegin();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2(250, 250));

        ImGui::Begin("Wave Controls", nullptr, ImGuiWindowFlags_NoResize);
        ImGui::Text("Wave Visualizer");
        ImGui::Separator();
        ImGui::Combo("Wave Type", (int *)&selectedWave, waveNames, WAVE_COUNT);
        ImGui::SliderFloat("Frequency", &frequency, 0.1f, 10.0f);
        ImGui::SliderFloat("Amplitude", &amplitude, 10.0f, 200.0f);
        ImGui::SliderFloat("Speed", &speed, 0.1f, 8.0f);
        ImGui::End();

        rlImGuiEnd();
        time += GetFrameTime() * speed * 100.0f;
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

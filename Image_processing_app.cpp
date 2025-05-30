// Interactive Image Processing App using Raylib + ImGui (Radio Buttons)
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <array>
using namespace std;

enum class ImageProcess
{
    None = 0,
    ColorGrayscale,
    ColorTint,
    ColorInvert,
    ColorContrast,
    ColorBrightness,
    FlipVertical,
    FlipHorizontal,
    Count
};

class PixelMorphApp
{
private:
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;

    const float MENU_X = 20.0f;
    const float MENU_Y = 20.0f;
    const float MENU_WIDTH = 200.0f;

    const array<const char *, static_cast<size_t>(ImageProcess::Count)> PROCESS_TEXT = {
        "NO PROCESSING",
        "COLOR GRAYSCALE",
        "COLOR TINT",
        "COLOR INVERT",
        "COLOR CONTRAST",
        "COLOR BRIGHTNESS",
        "FLIP VERTICAL",
        "FLIP HORIZONTAL"};

    Image originalImage{};
    Image processedImage{};
    Texture2D texture{};

    int currentProcessIdx = 0;

public:
    PixelMorphApp() = default;
    ~PixelMorphApp() { UnloadResources(); }

    void Run()
    {
        InitWindow(800, 450, "PixelMorph");
        SetupScreen();
        SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        SetWindowPosition((GetMonitorWidth(GetCurrentMonitor()) - SCREEN_WIDTH) / 2,
                          (GetMonitorHeight(GetCurrentMonitor()) - SCREEN_HEIGHT) / 2);
        SetWindowTitle("PixelMorph");
        SetTargetFPS(60);
        rlImGuiSetup(true);

        LoadResources();
        ApplyImageProcessing();

        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // ImGui UI
            rlImGuiBegin();
            ImGui::SetNextWindowPos(ImVec2(MENU_X, MENU_Y));
            ImGui::SetNextWindowSize(ImVec2(MENU_WIDTH, PROCESS_TEXT.size() * 25 + 60));
            ImGui::Begin("Image Processing", nullptr,
                         ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoCollapse);

            ImGui::Text("Select Process:");
            ImGui::Separator();
            for (int i = 0; i < (int)PROCESS_TEXT.size(); ++i)
            {
                if (ImGui::RadioButton(PROCESS_TEXT[i], currentProcessIdx == i))
                {
                    currentProcessIdx = i;
                    ApplyImageProcessing();
                }
            }

            ImGui::End();
            rlImGuiEnd();

            DrawImage();
            EndDrawing();
        }

        rlImGuiShutdown();
    }

private:
    void SetupScreen()
    {
        int monitor = GetCurrentMonitor();
        int mw = GetMonitorWidth(monitor);
        int mh = GetMonitorHeight(monitor);
        const int margin = 160;
        SCREEN_WIDTH = max(640, mw - margin);
        SCREEN_HEIGHT = max(480, mh - margin);
    }

    void LoadResources()
    {
        originalImage = LoadImage("Assets/Mahayami.png");
        ImageFormat(&originalImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

        // Resize if too large
        if (originalImage.width > SCREEN_WIDTH / 2 || originalImage.height > SCREEN_HEIGHT / 2)
        {
            float sfw = (SCREEN_WIDTH / 2.0f) / originalImage.width;
            float sfh = (SCREEN_HEIGHT / 2.0f) / originalImage.height;
            float sf = min(sfw, sfh);
            ImageResize(&originalImage, (int)(originalImage.width * sf), (int)(originalImage.height * sf));
        }
        processedImage = ImageCopy(originalImage);
        texture = LoadTextureFromImage(processedImage);
    }

    void UnloadResources()
    {
        UnloadTexture(texture);
        UnloadImage(originalImage);
        UnloadImage(processedImage);
        CloseWindow();
    }

    void ApplyImageProcessing()
    {
        UnloadImage(processedImage);
        processedImage = ImageCopy(originalImage);
        switch (static_cast<ImageProcess>(currentProcessIdx))
        {
        case ImageProcess::ColorGrayscale:
            ImageColorGrayscale(&processedImage);
            break;
        case ImageProcess::ColorTint:
            ImageColorTint(&processedImage, GREEN);
            break;
        case ImageProcess::ColorInvert:
            ImageColorInvert(&processedImage);
            break;
        case ImageProcess::ColorContrast:
            ImageColorContrast(&processedImage, -40);
            break;
        case ImageProcess::ColorBrightness:
            ImageColorBrightness(&processedImage, -80);
            break;
        case ImageProcess::FlipVertical:
            ImageFlipVertical(&processedImage);
            break;
        case ImageProcess::FlipHorizontal:
            ImageFlipHorizontal(&processedImage);
            break;
        default:
            break;
        }
        Color *pixels = LoadImageColors(processedImage);
        UpdateTexture(texture, pixels);
        UnloadImageColors(pixels);
    }

    void DrawImage()
    {
        float margin = 20.0f;
        float availW = SCREEN_WIDTH - (MENU_X + MENU_WIDTH + 4 * margin);
        float availH = SCREEN_HEIGHT - 3 * margin;
        float ar = (float)texture.width / texture.height;
        float dw = availW;
        float dh = dw / ar;
        if (dh > availH)
        {
            dh = availH;
            dw = dh * ar;
        }
        Vector2 pos = {MENU_X + MENU_WIDTH + 2 * margin + (availW - dw) / 2,
                       margin + (availH - dh) / 2};
        DrawTexturePro(texture,
                       {0, 0, (float)texture.width, (float)texture.height},
                       {pos.x, pos.y, dw, dh},
                       {0, 0}, 0.0f, WHITE);
        DrawRectangleLinesEx({pos.x, pos.y, dw, dh}, 2, BLACK);
    }
};

int main()
{
    PixelMorphApp app;
    app.Run();
    return 0;
}

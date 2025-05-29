// Tabs in imgui
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

        ImGui::SetNextWindowSize(ImVec2(300, 200)); // Set fixed window size
        ImGui::Begin("Items", nullptr, ImGuiWindowFlags_NoResize);
        no resize

            if (ImGui::BeginTabBar("MyTabBar"))
        {
            if (ImGui::BeginTabItem("Fruits"))
            {
                const char *fruits[] = {
                    "Apple", "Banana", "Orange", "Mango",
                    "Grape", "Pineapple", "Strawberry", "Watermelon"};

                for (const char *fruit : fruits)
                {
                    ImGui::Bullet();
                    ImGui::Text("%s", fruit);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Vegetables"))
            {
                const char *vegetables[] = {
                    "Tomato", "Carrot", "Broccoli", "Spinach",
                    "Cucumber", "Bell Pepper", "Onion", "Potato"};

                for (const char *veg : vegetables)
                {
                    ImGui::Bullet();
                    ImGui::Text("%s", veg);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Chocolates"))
            {
                const char *chocolates[] = {
                    "Kitkat", "Dairy Milk", "Mars", "Snickers",
                    "Bounty", "Twix", "Toblerone", "Hershey's"};

                for (const char *choc : chocolates)
                {
                    ImGui::Bullet();
                    ImGui::Text("%s", choc);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Dry Fruits"))
            {
                const char *dryFruits[] = {
                    "Almonds", "Cashews", "Raisins", "Pistachios",
                    "Walnuts", "Dates", "Figs", "Apricots"};

                for (const char *dry : dryFruits)
                {
                    ImGui::Bullet();
                    ImGui::Text("%s", dry);
                }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

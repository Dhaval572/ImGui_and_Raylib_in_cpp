// Tic Tac Toe with Imgui
#include <imgui.h>
#include <rlImGui.h>
#include <string>

const int GRID_SIZE = 3;
char board[GRID_SIZE][GRID_SIZE] = {0};
char currentPlayer = 'X';
bool gameOver = false;
char winner = 0;

void ResetGame()
{
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            board[i][j] = 0;

    currentPlayer = 'X';
    gameOver = false;
    winner = 0;
}

bool CheckWinner()
{
    // Rows, columns
    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            winner = board[i][0];
            return true;
        }
        if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            winner = board[0][i];
            return true;
        }
    }

    // Diagonals
    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        winner = board[0][0];
        return true;
    }

    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        winner = board[0][2];
        return true;
    }

    // Check for draw
    bool full = true;
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            if (board[i][j] == 0)
                full = false;

    if (full)
    {
        winner = '-';
        return true;
    }

    return false;
}

int main()
{
    InitWindow(340, 410, "Tic Tac Toe - raylib + ImGui");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        rlImGuiBegin();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin("Tic Tac Toe", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

        if (!gameOver)
            ImGui::Text("Current Player: %c", currentPlayer);
        else if (winner == '-')
            ImGui::Text("Game Over: It's a Draw!");
        else
            ImGui::Text("Winner: %c", winner);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Grid layout
        for (int row = 0; row < GRID_SIZE; row++)
        {
            for (int col = 0; col < GRID_SIZE; col++)
            {
                ImGui::PushID(row * GRID_SIZE + col);
                ImVec2 buttonSize = ImVec2(100, 100);

                if (ImGui::Button(board[row][col] == 0 ? " " : std::string(1, board[row][col]).c_str(), buttonSize) && !gameOver)
                {
                    if (board[row][col] == 0)
                    {
                        board[row][col] = currentPlayer;
                        if (CheckWinner())
                        {
                            gameOver = true;
                        }
                        else
                        {
                            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                        }
                    }
                }

                ImGui::PopID();

                if (col < GRID_SIZE - 1)
                    ImGui::SameLine();
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();
        if (ImGui::Button("Reset Game"))
        {
            ResetGame();
        }

        ImGui::End();
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

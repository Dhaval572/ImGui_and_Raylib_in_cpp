#pragma once
#include "SudokuBoard.h"
#include <imgui.h>
#include <algorithm>
class SudokuBoard; 

class SudokuRenderer 
{
private:
    static constexpr float CELL_SIZE = 70.0f;
    static constexpr float NORMAL_SPACING = 4.0f;
    static constexpr float BLOCK_SPACING = 15.0f;
    static constexpr float BUTTON_WIDTH = 120.0f;
    static constexpr float BUTTON_HEIGHT = 40.0f;
    static constexpr float BORDER_THICKNESS = 3.0f;

    // Color scheme
    struct Colors 
	{
        static constexpr ImVec4 DARK_BLOCK{0.15f, 0.15f, 0.2f, 1.0f};
        static constexpr ImVec4 LIGHT_BLOCK{0.2f, 0.2f, 0.25f, 1.0f};
        static constexpr ImVec4 SELECTED{0.2f, 0.5f, 1.0f, 1.0f};
        static constexpr ImVec4 ROW_COL_HIGHLIGHT_DARK
		{
			0.25f, 0.35f, 0.5f, 1.0f
		};
        static constexpr ImVec4 ROW_COL_HIGHLIGHT_LIGHT
		{
			0.3f, 0.4f, 0.6f, 1.0f
		};
        static constexpr ImVec4 BLOCK_HIGHLIGHT_DARK{0.2f, 0.25f, 0.35f, 1.0f};
        static constexpr ImVec4 BLOCK_HIGHLIGHT_LIGHT{0.25f, 0.3f, 0.4f, 1.0f};
        static constexpr ImVec4 ERROR_CELL{0.8f, 0.2f, 0.2f, 1.0f};
        static constexpr ImVec4 TEXT{1.0f, 1.0f, 1.0f, 1.0f};
        static constexpr ImVec4 ERROR_TEXT{1.0f, 1.0f, 0.2f, 1.0f};
        static constexpr ImVec4 INSTRUCTION{0.7f, 0.9f, 1.0f, 1.0f};
        static constexpr ImVec4 BORDER{0.4f, 0.4f, 0.5f, 1.0f};
    };

    ImVec4 GetCellColor(int row, int col, const SudokuBoard& board) const;
    void SetupCellStyle(const ImVec4& cell_color, bool b_HasError = false) const;

public:
    bool RenderGrid(SudokuBoard& board) const;
    void RenderControls(SudokuBoard& board) const;
	float CalculateScaleFactor(const ImVec2& window_size) const;
};
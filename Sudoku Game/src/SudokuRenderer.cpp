#include "SudokuRenderer.h"

ImVec4 SudokuRenderer::GetCellColor
(
	int row, 
	int col, 
	const SudokuBoard &board
) const
{
	char value = board.GetValue(row, col);

	// Check for errors first
	if (value != '\0' && !board.b_IsValidPlacement(row, col, value))
	{
		return Colors::ERROR_CELL;
	}

	int block_row = row / 3;
	int block_col = col / 3;
	bool b_IsDarkBlock = (block_row + block_col) % 2 == 0;

	int sel_row = board.GetSelectedRow();
	int sel_col = board.GetSelectedCol();

	if (sel_row == row && sel_col == col)
	{
		return Colors::SELECTED;
	}
	else if (sel_row == row || sel_col == col)
	{
		return b_IsDarkBlock ? Colors::ROW_COL_HIGHLIGHT_DARK : Colors::ROW_COL_HIGHLIGHT_LIGHT;
	}
	else if (board.b_HasSelection() 
				&& board.b_IsInSameBlock(sel_row, sel_col, row, col))
	{
		return b_IsDarkBlock ? Colors::BLOCK_HIGHLIGHT_DARK : Colors::BLOCK_HIGHLIGHT_LIGHT;
	}
	else
	{
		return b_IsDarkBlock ? Colors::DARK_BLOCK : Colors::LIGHT_BLOCK;
	}
}

void SudokuRenderer::SetupCellStyle
(
	const ImVec4 &cell_color, 
	bool b_HasError
) const
{
	ImGui::PushStyleColor(ImGuiCol_FrameBg, cell_color);
	ImGui::PushStyleColor
	(
		ImGuiCol_FrameBgHovered, 
		ImVec4
		(
			cell_color.x + 0.1f, 
			cell_color.y + 0.1f, 
			cell_color.z + 0.2f, 
			1.0f
		)
	);
	ImGui::PushStyleColor
	(
		ImGuiCol_FrameBgActive, 
		ImVec4
		(
			0.3f, 0.6f, 1.0f, 1.0f
		)
	);
	ImGui::PushStyleColor
	(
		ImGuiCol_Text, 
		b_HasError ? Colors::ERROR_TEXT : Colors::TEXT
	);
}

bool SudokuRenderer::RenderGrid(SudokuBoard &board) const
{
    bool board_changed = false;
    ImVec2 window_size = ImGui::GetWindowSize();
    float scale_factor = CalculateScaleFactor(window_size);

    // Responsive cell sizing
    float responsive_cell_size = CELL_SIZE * scale_factor;
    float responsive_normal_spacing = NORMAL_SPACING * scale_factor;
    float responsive_block_spacing = BLOCK_SPACING * scale_factor;

    // Calculate board dimensions with responsive spacing
    float total_cell_width = 9 * responsive_cell_size;
    float total_normal_spacing = 6 * responsive_normal_spacing;
    float total_block_spacing = 2 * responsive_block_spacing;

    // Responsive padding
    float responsive_padding = 40.0f * scale_factor;
    float board_width = total_cell_width + total_normal_spacing + total_block_spacing + responsive_padding;
    float board_height = total_cell_width + total_normal_spacing + total_block_spacing;

    // Ensure board doesn't exceed window bounds
    float max_board_width = window_size.x * 0.9f;
    float max_board_height = window_size.y * 0.7f;
    
    if (board_width > max_board_width)
    {
        float scale_down = max_board_width / board_width;
        responsive_cell_size *= scale_down;
        responsive_normal_spacing *= scale_down;
        responsive_block_spacing *= scale_down;
        responsive_padding *= scale_down;
        
        // Recalculate with scaled values
        total_cell_width = 9 * responsive_cell_size;
        total_normal_spacing = 6 * responsive_normal_spacing;
        total_block_spacing = 2 * responsive_block_spacing;
        board_width = total_cell_width + total_normal_spacing + total_block_spacing + responsive_padding;
        board_height = total_cell_width + total_normal_spacing + total_block_spacing;
    }
    
    if (board_height > max_board_height)
    {
        float scale_down = max_board_height / board_height;
        responsive_cell_size *= scale_down;
        responsive_normal_spacing *= scale_down;
        responsive_block_spacing *= scale_down;
        
        // Recalculate with scaled values
        total_cell_width = 9 * responsive_cell_size;
        total_normal_spacing = 6 * responsive_normal_spacing;
        total_block_spacing = 2 * responsive_block_spacing;
        board_width = total_cell_width + total_normal_spacing + total_block_spacing + responsive_padding;
        board_height = total_cell_width + total_normal_spacing + total_block_spacing;
    }

    // Center the board
    float board_start_x = (window_size.x - board_width) * 0.5f;
    if (board_start_x > 0)
    {
        ImGui::SetCursorPosX(board_start_x);
    }

    // Create responsive container
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.12f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, Colors::BORDER);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 15.0f * scale_factor);
    ImGui::PushStyleVar
    (
        ImGuiStyleVar_ChildBorderSize, 
        BORDER_THICKNESS * scale_factor
    );

    ImGui::PushStyleVar
    (
        ImGuiStyleVar_WindowPadding, 
        ImVec2
        (
            20 * scale_factor, 
            20 * scale_factor
        )
    );

    ImGui::BeginChild
    (
        "SudokuContainer", 
        ImVec2
        (
            board_width, 
            board_height
        ), 
        true, 
        ImGuiWindowFlags_NoScrollbar
    );

    // Responsive grid styling
    ImGui::PushStyleVar
    (
        ImGuiStyleVar_FramePadding, 
        ImVec2
        (
            15 * scale_factor, 
            15 * scale_factor
        )
    );

    ImGui::PushStyleVar
    (
        ImGuiStyleVar_ItemSpacing, 
        ImVec2
        (
            responsive_normal_spacing, 
            responsive_normal_spacing
        )
    );

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f * scale_factor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f * scale_factor);

    // Persistent input buffers to prevent losing edits when focus changes.
    // Static to persist across frames. Indexed by [row][col][2].
    static char cell_buffers[9][9][2];
    static bool b_BuffersInitialized = false;
    if (!b_BuffersInitialized)
    {
        std::memset(cell_buffers, 0, sizeof(cell_buffers));
        for (int r = 0; r < 9; ++r)
        {
            for (int c = 0; c < 9; ++c)
            {
                char v = board.GetValue(r, c);

                if (v != '\0')
                {
                    cell_buffers[r][c][0] = v;
                }

                cell_buffers[r][c][1] = '\0';
            }
        }
        b_BuffersInitialized = true;
    }

    for (int row = 0; row < 9; ++row)
    {
        // Add spacing between 3x3 blocks
        if (row > 0 && row % 3 == 0)
        {
            ImGui::Dummy(ImVec2(0, responsive_block_spacing - responsive_normal_spacing));
        }

        for (int col = 0; col < 9; ++col)
        {
            ImGui::PushID(row * 9 + col);

            char cell_value = board.GetValue(row, col);
            bool b_HasError = cell_value != '\0' 
                && !board.b_IsValidPlacement(row, col, cell_value);

            ImVec4 cell_color = GetCellColor(row, col, board);
            if (b_HasError)
            {
                cell_color = Colors::ERROR_CELL;
            }

            SetupCellStyle(cell_color, b_HasError);

            ImGui::SetNextItemWidth(responsive_cell_size);

            // Sync buffer from board for cells that are NOT currently selected.
            // This keeps buffers up-to-date after external changes (ClearAll, solver, etc.)
            int sel_row = board.GetSelectedRow();
            int sel_col = board.GetSelectedCol();
            if (!(sel_row == row && sel_col == col))
            {
                // Only overwrite if different to avoid disturbing an in-progress edit
                char board_val = board.GetValue(row, col);

                char buf_val = cell_buffers[row][col][0];
                if (board_val != buf_val)
                {
                    if (board_val != '\0')
                    {
                        cell_buffers[row][col][0] = board_val;
                    }
                    else
                    {
                        cell_buffers[row][col][0] = '\0';
                    }
                        
                    cell_buffers[row][col][1] = '\0';
                }
            }

            // Use a unique ID for each input field to prevent text sharing
            char input_id[16];
            std::snprintf
            (
                input_id, 
                sizeof(input_id), 
                "##cell_%d_%d", 
                row, 
                col
            );

            ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsDecimal | 
                                        ImGuiInputTextFlags_CharsNoBlank |
                                        ImGuiInputTextFlags_AutoSelectAll |
                                        ImGuiInputTextFlags_EnterReturnsTrue;

            // Render the input using the persistent buffer
            if (ImGui::InputText(input_id, cell_buffers[row][col], 2, flags))
            {
                // Commit change immediately whenever buffer differs from board
                char new_value = cell_buffers[row][col][0];
                if (new_value >= '1' && new_value <= '9')
                {
                    if (board.GetValue(row, col) != new_value)
                    {
                        board.b_SetValue(row, col, new_value);
                        board_changed = true;
                    }
                }
                else
                {
                    // If buffer was cleared or contains invalid char, clear board cell
                    if (board.GetValue(row, col) != '\0')
                    {
                        board.b_SetValue(row, col, '\0');
                        board_changed = true;
                    }
                    // Ensure buffer is normalized to empty
                    cell_buffers[row][col][0] = '\0';
                    cell_buffers[row][col][1] = '\0';
                }
            }
            else
            {
                // If InputText didn't return true but the user typed (buffer changed),
                // detect and commit as well. This helps in cases where InputText updates
                // without returning true (platform differences).
                char buf_val = cell_buffers[row][col][0];
                if (buf_val != cell_value)
                {
                    if (buf_val >= '1' && buf_val <= '9')
                    {
                        board.b_SetValue(row, col, buf_val);
                        board_changed = true;
                    }
                    else if (buf_val == '\0')
                    {
                        board.b_SetValue(row, col, '\0');
                        board_changed = true;
                    }
                }
            }

            // Handle selection
            if (ImGui::IsItemActive() 
                || ImGui::IsItemFocused() 
                || ImGui::IsItemClicked())
            {
                board.SetSelection(row, col);
            }

            ImGui::PopStyleColor(4);
            ImGui::PopID();

            // Horizontal spacing
            if (col < 8)
            {
                if ((col + 1) % 3 == 0)
                {
                    ImGui::SameLine(0, responsive_block_spacing);
                }
                else
                {
                    ImGui::SameLine(0, responsive_normal_spacing);
                }
            }
        }
    }

    ImGui::PopStyleVar(4);
    ImGui::EndChild();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    return board_changed;
}

void SudokuRenderer::RenderControls(SudokuBoard &board) const
{
    ImGui::Spacing();
    ImGui::Spacing();

    ImVec2 window_size = ImGui::GetWindowSize();
    float scale_factor = CalculateScaleFactor(window_size);

    // Responsive button sizing
    float responsive_button_width = BUTTON_WIDTH * scale_factor;
    float responsive_button_height = BUTTON_HEIGHT * scale_factor;

    // Center buttons responsively
    float total_button_width = responsive_button_width * 3 + ImGui::GetStyle().ItemSpacing.x * 2;

    float button_start_x = (window_size.x - total_button_width) * 0.5f;
    
    if (button_start_x > 0)
    {
        ImGui::SetCursorPosX(button_start_x);
    }

    // Stack buttons vertically on small screens
    if (window_size.x < 500)
    {
        // Clear All button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor
        (
            ImGuiCol_ButtonHovered, 
            ImVec4
            (
                1.0f, 0.3f, 0.3f, 1.0f
            )
        );

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f * scale_factor);
        
        if (ImGui::Button("Clear All", 
            ImVec2
            (
                responsive_button_width, 
                responsive_button_height
            )))
        {
            board.ClearAll();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        // Solve button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor
        (
            ImGuiCol_ButtonHovered, 
            ImVec4
            (
                0.3f, 0.8f, 0.3f, 1.0f
            )
        );

        ImGui::PushStyleVar
        (
            ImGuiStyleVar_FrameRounding, 
            8.0f * scale_factor
        );

        if (ImGui::Button("Solve", 
            ImVec2
            (
                responsive_button_width, 
                responsive_button_height
            )))
        {
            ImGui::OpenPopup("Solve Feature");
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        // Validate button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.7f, 1.0f));
        ImGui::PushStyleColor
        (
            ImGuiCol_ButtonHovered, 
            ImVec4
            (
                0.3f, 0.3f, 0.8f, 1.0f
            )
        );

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f * scale_factor);
        if (ImGui::Button("Validate", 
            ImVec2
            (
                responsive_button_width, 
                responsive_button_height
            )))
        {
            ImGui::OpenPopup("Validate Feature");
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
    }
    else
    {
        // Horizontal layout for larger screens
        // Clear All button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor
        (
            ImGuiCol_ButtonHovered, 
            ImVec4
            (
                1.0f, 0.3f, 0.3f, 1.0f
            )
        );

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f * scale_factor);
        if (ImGui::Button("Clear All", 
            ImVec2
            (
                responsive_button_width, 
                responsive_button_height
            )))
        {
            board.ClearAll();
        }
        
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        ImGui::SameLine();

        // Solve button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor
        (
            ImGuiCol_ButtonHovered, 
            ImVec4
            (
                0.3f, 0.8f, 0.3f, 1.0f
            )
        );

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f * scale_factor);

        if (ImGui::Button("Solve", 
            ImVec2
            (
                responsive_button_width, 
                responsive_button_height
            )))
        {
            ImGui::OpenPopup("Solve Feature");
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        ImGui::SameLine();

        // Validate button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.7f, 1.0f));
        ImGui::PushStyleColor
        (
            ImGuiCol_ButtonHovered, 
            ImVec4
            (
                0.3f, 0.3f, 0.8f, 1.0f
            )
        );

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f * scale_factor);
        if (ImGui::Button("Validate", 
            ImVec2
            (
                responsive_button_width, 
                responsive_button_height
            )))
        {
            ImGui::OpenPopup("Validate Feature");
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
    }

    // Responsive popup modals
    if (ImGui::BeginPopupModal("Solve Feature", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Solver algorithm coming soon!");
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(120 * scale_factor, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Validate Feature", 
        NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Validation feature coming soon!");
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(120 * scale_factor, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

float SudokuRenderer::CalculateScaleFactor(const ImVec2& windowSize) const
{
    // Base reference size (typical desktop window)
    constexpr float BASE_WIDTH = 800.0f;
    constexpr float BASE_HEIGHT = 600.0f;
    
    float w_scale = windowSize.x / BASE_WIDTH;
    float h_scale = windowSize.y / BASE_HEIGHT;
    
    // Make scalling perfect 
    float scale = std::min(w_scale, h_scale);
    return std::clamp(scale, 0.5f, 2.0f);
}
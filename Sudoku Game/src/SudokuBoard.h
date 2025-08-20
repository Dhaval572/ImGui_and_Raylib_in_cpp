#pragma once
#include <array>

class SudokuBoard
{
private:
	std::array<std::array<char, 10>, 9> m_GridArray;
	int m_SelectedRow, m_SelectedCol;

public:
	SudokuBoard();

	// Core board operations
	void ClearAll();
	char GetValue(int row, int col) const;
	bool b_SetValue(int row, int col, char value);

	// Selection management
	void SetSelection(int row, int col);
	void ClearSelection();
	int GetSelectedRow() const;
	int GetSelectedCol() const;
	bool b_HasSelection() const;

	// Get raw grid pointer for ImGui input
	char *GetCellPtr(int row, int col);

	// Game logic helpers
	bool b_IsInSameBlock(int row1, int col1, int row2, int col2) const;
	bool b_IsEmpty() const;

	// Validation
	bool b_HasDuplicateInRow(int row, char value) const;
	bool b_HasDuplicateInCol(int col, char value) const;
	bool b_HasDuplicateInBlock(int row, int col, char value) const;
	bool b_IsValidPlacement(int row, int col, char value) const;
};
#include "SudokuBoard.h"

SudokuBoard::SudokuBoard() 
	: m_SelectedRow(-1), m_SelectedCol(-1)
{ 
	ClearAll();
}

void SudokuBoard::ClearAll()
{
	for (auto &row : m_GridArray)
	{
		for (auto &cell : row)
		{
			cell = '\0';
		}
	}
	m_SelectedRow = m_SelectedCol = -1;
}

bool SudokuBoard::b_SetValue(int row, int col, char value)
{
	if (row < 0 || row >= 9 || col < 0 || col >= 9)
	{
		return false;
	}

	if (value != '\0' && (value < '1' || value > '9'))
	{
		return false;
	}

	m_GridArray[row][col] = value;
	return true;
}

char SudokuBoard::GetValue(int row, int col) const
{
	if (row < 0 || row >= 9 || col < 0 || col >= 9)
	{
		return '\0';
	}

	return m_GridArray[row][col];
}

void SudokuBoard::SetSelection(int row, int col)
{
	m_SelectedRow = (row >= 0 && row < 9 && col >= 0 && col < 9) ? row : -1;
	m_SelectedCol = (row >= 0 && row < 9 && col >= 0 && col < 9) ? col : -1;
}

void SudokuBoard::ClearSelection()
{
	m_SelectedRow = m_SelectedCol = -1;
}

int SudokuBoard::GetSelectedRow() const
{
	return m_SelectedRow;
}

int SudokuBoard::GetSelectedCol() const
{
	return m_SelectedCol;
}

bool SudokuBoard::b_HasSelection() const
{
	return m_SelectedRow >= 0 && m_SelectedCol >= 0;
}

char *SudokuBoard::GetCellPtr(int row, int col)
{
	if (row < 0 || row >= 9 || col < 0 || col >= 9)
	{
		return nullptr;
	}

	return &m_GridArray[row][col];
}

bool SudokuBoard::b_IsInSameBlock(int row1, int col1, int row2, int col2) const
{
	return (row1 / 3 == row2 / 3) && (col1 / 3 == col2 / 3);
}

bool SudokuBoard::b_IsEmpty() const
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (m_GridArray[i][j] != '\0')
			{
				return false;
			}
		}
	}
	return true;
}

bool SudokuBoard::b_HasDuplicateInRow(int row, char value) const
{
	if (value == '\0')
	{
		return false;
	}

	int count = 0;
	for (int col = 0; col < 9; col++)
	{
		if (m_GridArray[row][col] == value)
		{
			count++;
		}
	}
	return count > 1;
}

bool SudokuBoard::b_HasDuplicateInCol(int col, char value) const
{
	if (value == '\0')
	{
		return false;
	}

	int count = 0;
	for (int row = 0; row < 9; row++)
	{
		if (m_GridArray[row][col] == value)
		{
			count++;
		}
	}
	return count > 1;
}

bool SudokuBoard::b_HasDuplicateInBlock(int row, int col, char value) const
{
	if (value == '\0')
	{
		return false;
	}

	int count = 0;
	int block_start_row = (row / 3) * 3;
	int block_start_col = (col / 3) * 3;

	for (int r = block_start_row; r < block_start_row + 3; r++)
	{
		for (int c = block_start_col; c < block_start_col + 3; c++)
		{
			if (m_GridArray[r][c] == value)
			{
				count++;
			}
		}
	}
	return count > 1;
}

bool SudokuBoard::b_IsValidPlacement(int row, int col, char value) const
{
	return !b_HasDuplicateInRow(row, value) &&
		   !b_HasDuplicateInCol(col, value) &&
		   !b_HasDuplicateInBlock(row, col, value);
}
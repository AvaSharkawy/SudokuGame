#include <iostream>
#include <math.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <direct.h>
#include <filesystem>
#include <fstream>

#define KEY_BACK 8
#define KEY_RETURN 13
#define KEY_ESC 27
#define KEY_NUMPAD0 48
#define KEY_NUMPAD1 49
#define KEY_NUMPAD2 50
#define KEY_NUMPAD3 51
#define KEY_NUMPAD4 52
#define KEY_NUMPAD5 53
#define KEY_NUMPAD6 54
#define KEY_NUMPAD7 55
#define KEY_NUMPAD8 56
#define KEY_NUMPAD9 57
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80
#define KEY_B 98
#define KEY_C 99
#define KEY_H 104
#define KEY_R 114
#define KEY_S 115
#define KEY_U 117

using namespace std;
namespace fs = std::filesystem;

struct Node
{
	int data;
	Node* right, * down;
};

struct Position
{
	int Row, Column;
};

struct PlayerMove
{
	int PreviousValue, CurrentValue;
	Position Location;
};

struct SavedGame
{
	int ID;
	string Name, Difficulty, Progress;
};

class TD_LinkedList
{
private:
	Node* head, * RowCurrent, * ColCurrent;
	int  AllBlocks, BlockSize;
	Position BlocksHeads[25];
public:
	int SizeRows, SizeColumns;
	bool inputfinished = false;
	int totalnodes;

	TD_LinkedList()
	{

	}

	TD_LinkedList(int InputRows, int InputColumns)
	{
		totalnodes = InputRows * InputColumns;
		SizeRows = InputRows;
		SizeColumns = InputColumns;
		AllBlocks = InputRows;
		BlockSize = int(sqrt(double(InputRows)));
		head = new Node;
		for (int i = 0; i < InputRows; i++)
		{
			if (i == 0)
			{
				head->data = 0;
				head->down = new Node;
				head->right = new Node;
				ColCurrent = head->right;
				for (int j = 1; j < InputColumns; j++)
				{
					if (j != InputColumns - 1)
					{
						ColCurrent->data = 0;
						ColCurrent->right = new Node;
						ColCurrent = ColCurrent->right;
					}
					else
					{
						ColCurrent->data = 0;
						ColCurrent->right = NULL;
						ColCurrent = ColCurrent->right;
					}
				}

				RowCurrent = head->down;
			}
			else if (i != InputRows - 1)
			{
				RowCurrent->data = 0;
				RowCurrent->down = new Node;
				RowCurrent->right = new Node;
				ColCurrent = RowCurrent->right;
				for (int j = 1; j < InputColumns; j++)
				{
					if (j != InputColumns - 1)
					{
						ColCurrent->data = 0;
						ColCurrent->right = new Node;
						ColCurrent = ColCurrent->right;
					}
					else
					{
						ColCurrent->data = 0;
						ColCurrent->right = NULL;
						ColCurrent = ColCurrent->right;
					}
				}

				RowCurrent = RowCurrent->down;
			}
			else
			{
				RowCurrent->data = 0;
				RowCurrent->down = NULL;
				RowCurrent->right = new Node;
				ColCurrent = RowCurrent->right;
				for (int j = 1; j < InputColumns; j++)
				{
					if (j != InputColumns - 1)
					{
						ColCurrent->data = 0;
						ColCurrent->right = new Node;
						ColCurrent = ColCurrent->right;
					}
					else
					{
						ColCurrent->data = 0;
						ColCurrent->right = NULL;
						ColCurrent = ColCurrent->right;
					}
				}
				RowCurrent = RowCurrent->down;
			}
		}

		for (int k = 1; k < InputColumns; k++)
		{
			Node* n = GetNodeAtPosition(1, k + 1);
			for (int h = 2; h <= InputRows; h++)
			{
				if (h != InputRows)
				{
					n->down = GetNodeAtPosition(h, k + 1);
					n = n->down;
				}
				else
				{
					n->down = GetNodeAtPosition(h, k + 1);
					n = n->down;
					n->down = NULL;
				}
			}
		}

		int currentb = 0;
		for (int i = 1; i <= SizeRows; i += BlockSize)
		{
			for (int j = 1; j <= SizeColumns; j += BlockSize)
			{
				BlocksHeads[currentb].Row = i;
				BlocksHeads[currentb].Column = j;
				currentb++;
			}
		}
	}

	Node* GetHead()
	{
		return head;
	}

	void Display()
	{
		Node* Row = head;
		Node* Column = Row->right;

		string header = "-----";
		for (int i = 1; i < SizeColumns; i++)
		{
			header += "----";
		}

		cout << header << "\t\t\t\t\t|| \tMenu" << endl;

		for (int row = 1; row <= SizeRows; row++)
		{
			if (Row->data == 0)
				cout << "| " << " " << " | ";
			else
				cout << "| " << Row->data << " | ";


			string sb = "|---|";

			for (int col = 1; col < SizeColumns; col++)
			{
				if (col != SizeColumns - 1)
				{
					if (Column->data == 0)
					{
						cout << " " << " | ";
					}
					else
					{
						cout << Column->data << " | ";
					}
				}
				else
				{
					if (Column->data == 0)
					{
						if (row == 2)
						{
							cout << " " << " |\t\t\t\t\t|| C - Solve the game.";
						}
						else if (row == 3)
						{
							cout << " " << " |\t\t\t\t\t|| S - Save progress.";
						}
						else if (row == 4)
						{
							cout << " " << " |\t\t\t\t\t|| H - Solve a single cell.";
						}
						else if (row == 5)
						{
							cout << " " << " |\t\t\t\t\t|| U - Undo last move.";
						}
						else if (row == 6)
						{
							cout << " " << " |\t\t\t\t\t|| R - Redo last move.";
						}
						else if (row == 7)
						{
							cout << " " << " |\t\t\t\t\t|| B or Back button - Back to main menu.";
						}
						else
						{
							cout << " " << " |\t\t\t\t\t||";

						}
					}
					else
					{
						if (row == 2)
						{
							cout << Column->data << " |\t\t\t\t\t|| C - Solve the game.";
						}
						else if (row == 3)
						{
							cout << Column->data << " |\t\t\t\t\t|| S - Save progress.";
						}
						else if (row == 4)
						{
							cout << Column->data << " |\t\t\t\t\t|| H - Solve a single cell.";
						}
						else if (row == 5)
						{
							cout << Column->data << " |\t\t\t\t\t|| U - Undo last move.";
						}
						else if (row == 6)
						{
							cout << Column->data << " |\t\t\t\t\t|| R - Redo last move.";
						}
						else if (row == 7)
						{
							cout << Column->data << " |\t\t\t\t\t|| B or Back button - Back to main menu.";
						}
						else
						{
							cout << Column->data << " |\t\t\t\t\t||";
						}
					}
				}
				Column = Column->right;

				if (col != SizeColumns - 1)
				{
					sb += "---|";
				}
				else
				{
					sb += "---|\t\t\t\t\t||";
				}
			}
			cout << endl << sb << endl;
			Row = Row->down;
			if (Row != NULL)
			{
				Column = Row->right;
			}
		}
	}

	Node* GetNodeAtPosition(int iRow, int iColumn)
	{
		Node* n = NULL;
		if (iRow > SizeRows)
		{
			cout << "cannot get Node because of Out of Rows index" << endl;
		}
		else if (iColumn > SizeColumns)
		{
			cout << "cannot get Node because of Out of Columns index" << endl;
		}
		else if (iRow > SizeRows&& iColumn > SizeColumns)
		{
			cout << "cannot get Node because of Out of Rows and Columns index" << endl;
		}
		else
		{
			n = head;
			for (int i = 1; i < iRow; i++)
			{
				n = n->down;
			}

			for (int j = 1; j < iColumn; j++)
			{
				n = n->right;
			}
		}
		return n;
	}

	Node* GetNodeAtPosition(Node* FHead, int iRow, int iColumn)
	{
		Node* n = FHead;
		if (iRow > SizeRows)
		{
			cout << "cannot get Node because of Out of Rows index" << endl;
		}
		else if (iColumn > SizeColumns)
		{
			cout << "cannot get Node because of Out of Columns index" << endl;
		}
		else if (iRow > SizeRows&& iColumn > SizeColumns)
		{
			cout << "cannot get Node because of Out of Rows and Columns index" << endl;
		}
		else
		{
			n = FHead;
			for (int i = 1; i < iRow; i++)
			{
				n = n->down;
			}
			for (int j = 1; j < iColumn; j++)
			{
				n = n->right;
			}
		}
		return n;
	}

	void SetNodeValueAtPosition(int iRow, int iColumn, int value)
	{
		GetNodeAtPosition(iRow, iColumn)->data = value;
	}

	void DeleteNodeZero(int iRow, int iColumn)
	{
		SetNodeValueAtPosition(iRow, iColumn, 0);
	}

	void DeleteNodeAtPosition(int iRow, int iColumn)
	{
		if (iRow > SizeRows)
		{
			cout << "Delete Node failed because of Out of Rows index" << endl;
		}
		else if (iColumn > SizeColumns)
		{
			cout << "Delete Node failed because of Out of Columns index" << endl;
		}
		else if (iRow > SizeRows&& iColumn > SizeColumns)
		{
			cout << "Delete Node failed because of Out of Rows and Columns index" << endl;
		}
		else
		{
			if (iRow == SizeRows && iColumn == SizeColumns)
			{
				GetNodeAtPosition(iRow - 1, iColumn)->down = NULL;
				GetNodeAtPosition(iRow, iColumn - 1)->right = NULL;
			}
			else if (iRow == 1 && iColumn == 1)
			{
				Node* np = head;
				GetNodeAtPosition(iRow, iColumn + 1)->down = head->down;
				for (int i = 2; i < iColumn - 1; i++)
				{
					GetNodeAtPosition(iRow, iColumn + i)->down = GetNodeAtPosition(iRow, i);
				}
				head = GetNodeAtPosition(1, 2);
			}
			else if (iRow == 1 && iColumn != 1 && iColumn != SizeColumns)
			{
				GetNodeAtPosition(iRow, iColumn - 1)->right = GetNodeAtPosition(iRow, iColumn + 1);
				GetNodeAtPosition(iRow, iColumn + 1)->down = GetNodeAtPosition(iRow + 1, iColumn);
			}
			else if (iRow == 1 && iColumn != 1 && iColumn == SizeColumns)
			{
				GetNodeAtPosition(iRow, iColumn - 1)->right = NULL;
			}
			else
			{
				GetNodeAtPosition(iRow, iColumn - 1)->right = GetNodeAtPosition(iRow, iColumn + 1);
				GetNodeAtPosition(iRow - 1, iColumn)->down = GetNodeAtPosition(iRow, iColumn);
				GetNodeAtPosition(iRow - 1, iColumn + 1)->down = NULL;
			}
		}
	}

	bool ValidRow(int Row, int value)
	{
		Node* n = GetNodeAtPosition(Row, 1);

		for (int i = 0; i < SizeColumns; i++)
		{
			if (n->data == value)
			{
				return false;
			}
			else
			{
				n = n->right;
			}
		}
		return true;
	}

	bool ValidColumn(int column, int value)
	{
		Node* n = GetNodeAtPosition(1, column);

		for (int j = 0; j < SizeRows; j++)
		{
			if (n->data == value)
			{
				return false;
			}
			else
			{
				n = n->down;
			}
		}

		return true;
	}

	bool ValidBlock(int iRow, int iColumn, int value)
	{
		Node* n = GetNodeAtPosition(iRow, iColumn);
		for (int j = 0; j < BlockSize; j++)
		{
			for (int i = 0; i < BlockSize; i++)
			{
				if (n->data == value)
				{
					return false;
				}
				else
				{
					n = n->right;
				}
			}
			n = GetNodeAtPosition(iRow + j, iColumn);
			n = n->down;
		}
		return true;
	}

	bool CheckAllBlocks(int value)
	{
		for (int i = 1; i <= SizeRows - 2; i += 3)
		{
			for (int j = 1; j <= SizeColumns - 2; j += 3)
			{
				if (!ValidBlock(i, j, value))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool EmptyLocation(int& Row, int& Column)
	{
		for (Row = 1; Row <= SizeRows; Row++)
		{
			for (Column = 1; Column <= SizeColumns; Column++)
			{
				if (GetNodeAtPosition(Row, Column)->data == 0)
					return true;
			}
		}
		return false;
	}

	Position GetBlockHeadPosition(int row, int column)
	{
		for (int i = 0; i < AllBlocks; i++)
		{
			if ((row - BlocksHeads[i].Row >= 0 && row - BlocksHeads[i].Row < BlockSize) && (column - BlocksHeads[i].Column >= 0 && column - BlocksHeads[i].Column < BlockSize))
				return BlocksHeads[i];
		}
	}

	bool ValidLocation(int Row, int Column, int value)
	{
		if (GetNodeAtPosition(Row, Column)->data == value && !inputfinished)
		{
			return true;
		}
		else if (inputfinished)
		{
			if (ValidFinishedLocation(Row, Column, value))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (!ValidRow(Row, value))
			{
				return false;

			}
			if (!ValidColumn(Column, value))
			{
				return false;
			}

			Position BlockP = GetBlockHeadPosition(Row, Column);

			if (!ValidBlock(BlockP.Row, BlockP.Column, value))
			{
				return false;
			}
		}
		return true;
	}

	bool ValidFinishedRow(int Row, int value)
	{
		int count = 0;
		Node* n = GetNodeAtPosition(Row, 1);

		for (int i = 0; i < SizeColumns; i++)
		{
			if (n->data == value && !(count >= 1))
			{
				count++;
				n = n->right;
			}
			else if (n->data == value && count >= 1)
			{
				count = 0;
				return false;
			}
			else
			{
				n = n->right;
			}
		}
		return true;
	}

	bool ValidFinishedColumn(int Column, int value)
	{
		int count = 0;
		Node* n = GetNodeAtPosition(1, Column);

		for (int j = 0; j < SizeRows; j++)
		{
			if (n->data == value && !(count >= 1))
			{
				count++;
				n = n->down;
			}
			else if (n->data == value && count >= 1)
			{
				count = 0;
				return false;
			}
			else
			{
				n = n->down;
			}
		}

		return true;
	}

	bool ValidFinishedBlock(int iRow, int iColumn, int value)
	{
		int count = 0;
		Node* n = GetNodeAtPosition(iRow, iColumn);
		for (int j = 0; j < BlockSize; j++)
		{
			for (int i = 0; i < BlockSize; i++)
			{
				if (n->data == value && !(count >= 1))
				{
					count++;
					n = n->right;
				}
				else if (n->data == value && count >= 1)
				{
					count = 0;
					return false;
				}
				else
				{
					n = n->right;
				}
			}
			n = GetNodeAtPosition(iRow + j, iColumn);
			n = n->down;
		}
		return true;
	}

	bool ValidFinishedLocation(int Row, int Column, int value)
	{
		if (!ValidFinishedRow(Row, value))
		{
			return false;

		}
		if (!ValidFinishedColumn(Column, value))
		{
			return false;

		}

		Position BlockP = GetBlockHeadPosition(Row, Column);

		if (!ValidFinishedBlock(BlockP.Row, BlockP.Column, value))
		{
			return false;
		}
		return true;
	}

	void Clear()
	{
		Node* Row = head;
		Node* Column = Row->right;
		while (Row != NULL)
		{
			Row->data = 0;
			while (Column != NULL)
			{
				Column->data = 0;
				Column = Column->right;
			}
			Row = Row->down;
			if (Row != NULL)
			{
				Column = Row->right;
			}
		}
	}

};

class SodukoGame
{
private:
	TD_LinkedList ls, fg;
public:
	int RandomFinalNumbers[256];
	Position RandomFinalPositions[256];
	int TotalFilledPositions = 0;
	string GameDifficulty = "";
	string SavesNames[10];
	SavedGame sg[10];
	int SavesCount = 0;

	SodukoGame(int InputRows, int InputColumns)
	{
		ls = TD_LinkedList(InputRows, InputColumns);
		fg = TD_LinkedList(InputRows, InputColumns);
		_mkdir("Saves");
	}

	void GameDisplay()
	{
		ls.Display();
	}

	void AddValueAtPosition(int Row, int Column, int value)
	{
		ls.SetNodeValueAtPosition(Row, Column, value);
	}

	bool SolveSoduko()
	{
		int row, column;
		if (!ls.EmptyLocation(row, column))
		{
			return true;
		}
		else
		{
			for (int i = 1; i <= ls.SizeRows; i++)
			{
				if (ls.ValidLocation(row, column, i))
				{
					ls.SetNodeValueAtPosition(row, column, i);
					fg.SetNodeValueAtPosition(row, column, i);
					if (SolveSoduko())
					{
						return true;
					}
					ls.SetNodeValueAtPosition(row, column, 0);
					fg.SetNodeValueAtPosition(row, column, 0);
				}
			}
		}
		return false;
	}

	void GetRandomNumbers(int Numbers, Position RandPosArr[], int RandArr[])
	{
		//int RandArr[81];
		srand(time(NULL));
		int num = 0;
		for (int i = 0; i < Numbers; i++)
		{
			num = (rand() % ls.SizeRows) + 1;
			RandArr[i] = num;
			RandomFinalNumbers[i] = num;
		}
		RandPosArr[81];
		//Position RandPosArr[81];

		int RandRowsArr[81];
		int RandColsArr[81];
		for (int j = 0; j < Numbers; j++)
		{
			RandRowsArr[j] = (rand() % ls.SizeRows) + 1;
			RandColsArr[j] = (rand() % ls.SizeColumns) + 1;
			RandPosArr[j].Row = RandRowsArr[j];
			RandPosArr[j].Column = RandColsArr[j];

			RandomFinalPositions[j].Row = RandRowsArr[j];
			RandomFinalPositions[j].Column = RandColsArr[j];

		}
	}

	bool PositionExists(Position PosArr[], int size, Position PosToFind)
	{
		for (int i = 0; i < size; i++)
		{
			if (PosArr[i].Row == PosToFind.Row && PosArr[i].Column == PosToFind.Column)
			{
				return true;
			}
		}
		return false;
	}

	void GetFinalFilledPositions(Position PosArr[])
	{
		TotalFilledPositions = 0;
		for (int row = 1; row <= ls.SizeRows; row++)
		{
			for (int col = 1; col <= ls.SizeColumns; col++)
			{
				if (ls.GetNodeAtPosition(row, col)->data != 0)
				{
					PosArr[TotalFilledPositions].Row = row;
					PosArr[TotalFilledPositions].Column = col;
					TotalFilledPositions++;
				}
			}
		}
	}

	void FillNodesWithRandomNumbers(int NumbersCount)
	{
	poi:
		const int BeginSize = 15;
		int BeginRandArr[BeginSize];
		srand(time(NULL));
		int num = 0;
		for (int i = 0; i < BeginSize; i++)
		{
			num = (rand() % ls.SizeRows) + 1;
			BeginRandArr[i] = num;
		}

		Position BeginRandPos[BeginSize];

		for (int j = 0; j < BeginSize; j++)
		{
			BeginRandPos[j].Row = (rand() % ls.SizeRows) + 1;
			BeginRandPos[j].Column = (rand() % ls.SizeColumns) + 1;
		}

		for (int j = 0; j < BeginSize; j++)
		{
			if (ls.ValidLocation(BeginRandPos[j].Row, BeginRandPos[j].Column, BeginRandArr[j]))
			{
				ls.SetNodeValueAtPosition(BeginRandPos[j].Row, BeginRandPos[j].Column, BeginRandArr[j]);
				fg.SetNodeValueAtPosition(BeginRandPos[j].Row, BeginRandPos[j].Column, BeginRandArr[j]);
			}
		}

		if (SolveSoduko())
		{
			Position RandDelPos[256];
			srand(time(NULL));

			for (int i = 0; i < ls.totalnodes - NumbersCount; i++)
			{
				Position p;
				p.Row = (rand() % ls.SizeRows) + 1;
				p.Column = (rand() % ls.SizeColumns) + 1;
				if (PositionExists(RandDelPos, i, p))
				{
					i--;
					continue;
				}
				RandDelPos[i] = p;
			}

			for (int i = 0; i < ls.totalnodes - NumbersCount; i++)
			{
				ls.DeleteNodeZero(RandDelPos[i].Row, RandDelPos[i].Column);
			}

			GetFinalFilledPositions(RandomFinalPositions);
		}
		else
		{
			for (int i = 0; i < BeginSize; i++) {
				BeginRandArr[i] = NULL;
				BeginRandPos[i].Column = NULL;
				BeginRandPos[i].Row = NULL;
			}
			ls.Clear();
			goto poi;
		}
	}

	int counter = 0;

	bool CheckValidGame(int numbers)
	{
		if (counter == 5)
			return false;

		counter++;

		int RArr[81];
		Position PArr[81];
		GetRandomNumbers(numbers, PArr, RArr);

		for (int j = 1; j <= numbers; j++)
		{
			if (ls.ValidLocation(PArr[j - 1].Row, PArr[j - 1].Column, RArr[j - 1]))
			{
				ls.SetNodeValueAtPosition(PArr[j - 1].Row, PArr[j - 1].Column, RArr[j - 1]);
			}
		}

		if (SolveSoduko())
		{
			Clear();
			for (int j = 1; j <= numbers; j++)
			{
				if (ls.ValidLocation(PArr[j - 1].Row, PArr[j - 1].Column, RArr[j - 1]))
				{
					ls.SetNodeValueAtPosition(PArr[j - 1].Row, PArr[j - 1].Column, RArr[j - 1]);
				}
			}
			return true;
		}

		Clear();
		if (CheckValidGame(numbers))
			return true;


		return false;
	}

	void Clear()
	{
		ls.Clear();
	}

	bool InputFinished()
	{
		int row, column;
		if (ls.EmptyLocation(row, column))
		{
			ls.inputfinished = false;
			return false;
		}
		else
		{
			ls.inputfinished = true;
			return true;
		}
	}

	bool CheckWon()
	{
		if (InputFinished())
		{
			for (int row = 1; row <= ls.SizeRows; row++)
			{
				for (int col = 1; col <= ls.SizeColumns; col++)
				{
					if (!ls.ValidLocation(row, col, ls.GetNodeAtPosition(row, col)->data))
					{
						return false;
					}
				}
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	void StartGame(int DifficultyChoose)
	{
		if (DifficultyChoose == 1)
		{
			ls.Clear();
			ls.inputfinished = false;
			FillNodesWithRandomNumbers(50);
			GameDifficulty = "Easy";
		}
		else if (DifficultyChoose == 2)
		{
			ls.Clear();
			ls.inputfinished = false;
			FillNodesWithRandomNumbers(35);
			GameDifficulty = "Medium";
		}
		else if (DifficultyChoose == 3)
		{
			ls.Clear();
			ls.inputfinished = false;
			FillNodesWithRandomNumbers(21);
			GameDifficulty = "Hard";
		}

		system("CLS");
		PlayerMove pm;
		pm.CurrentValue = 0;
		pm.PreviousValue = 0;
		pm.Location.Row = 0;
		pm.Location.Column = 0;
		bool solvedbycode = false;


		float x = 2, y = 2;
		GameDisplay();
		gotoXY(x, y);

		int a = _getch();

		while (a != KEY_ESC && !CheckWon())
		{
			if (_kbhit())
			{
				a = _getch();
				if (a == KEY_C)
				{
					/*for (int row = 1; row <= ls.SizeRows; row++)
					{
						for (int col = 1; col <= ls.SizeColumns; col++)
						{
							Position p;
							p.Row = row;
							p.Column = col;
							if (!PositionExists(RandomFinalPositions, TotalFilledPositions, p))
							{
								ls.DeleteNodeZero(row, col);
							}
						}
					}
					if (SolveSoduko())
					{
						system("CLS");
						gotoXY(0, 0);
						GameDisplay();
						solvedbycode = true;
					}
					else
					{
						gotoXY(0, ls.SizeColumns * 2 + 2);
						cout << "No Solution found." << endl;
						system("PAUSE");
						MainMenu();
					}*/

					gotoXY(0, 0);
					fg.Display();
					solvedbycode = true;
					gotoXY(0, ls.SizeColumns * 2 + 2);
					cout << "Game solved successfully." << endl;
					system("PAUSE");
					MainMenu();
				}
				else if (a == KEY_H)
				{
					Position p;
					p.Row = y / 2;
					p.Column = ((x - 2) / 4) + 1;
					int value = fg.GetNodeAtPosition(p.Row, p.Column)->data;
					if (!PositionExists(RandomFinalPositions, TotalFilledPositions, p))
					{
						ls.SetNodeValueAtPosition(p.Row, p.Column, value);
						gotoXY(0, 0);
						GameDisplay();
						gotoXY(x, y);
					}
				}
				else if (a == KEY_U)
				{
					Position p;
					p.Row = y / 2;
					p.Column = ((x - 2) / 4) + 1;
					if (!PositionExists(RandomFinalPositions, TotalFilledPositions, p))
					{
						if (pm.Location.Row != 0 && pm.Location.Column != 0)
						{
							ls.SetNodeValueAtPosition(pm.Location.Row, pm.Location.Column, pm.PreviousValue);
							gotoXY(0, 0);
							GameDisplay();
							gotoXY(x, y);
						}
					}
				}
				else if (a == KEY_R)
				{
					Position p;
					p.Row = y / 2;
					p.Column = ((x - 2) / 4) + 1;
					if (!PositionExists(RandomFinalPositions, TotalFilledPositions, p))
					{
						if (pm.Location.Row != 0 && pm.Location.Column != 0)
						{
							ls.SetNodeValueAtPosition(pm.Location.Row, pm.Location.Column, pm.CurrentValue);
							gotoXY(0, 0);
							GameDisplay();
							gotoXY(x, y);
						}
					}
				}
				else if (a == KEY_S)
				{
					gotoXY(0, ls.SizeColumns * 2 + 2);
					string name;
					cout << "Enter save name : ";
					cin >> name;
					SaveProgress(name);
					gotoXY(x, y);
				}
				else if (a == KEY_B || a == KEY_BACK)
				{
					gotoXY(0, 0);
					MainMenu();
				}
				else if (a == KEY_NUMPAD0 || a == KEY_NUMPAD1 || a == KEY_NUMPAD2 || a == KEY_NUMPAD3 || a == KEY_NUMPAD4 || a == KEY_NUMPAD5 || a == KEY_NUMPAD6 || a == KEY_NUMPAD7 || a == KEY_NUMPAD8 || a == KEY_NUMPAD9)
				{
					Position p;
					p.Row = y / 2;
					p.Column = ((x - 2) / 4) + 1;
					if (!PositionExists(RandomFinalPositions, TotalFilledPositions, p))
					{
						pm.PreviousValue = ls.GetNodeAtPosition(p.Row, p.Column)->data;
						ls.SetNodeValueAtPosition(p.Row, p.Column, a - 48);
						pm.CurrentValue = a - 48;
						pm.Location = p;
						gotoXY(0, 0);
						GameDisplay();
						gotoXY(0, ls.SizeColumns * 2 + 2);
						cout << "                                                                    " << endl;
						gotoXY(0, ls.SizeColumns * 2 + 3);
						cout << "                                                                    " << endl;
						gotoXY(x, y);
					}
				}
				else if (a == KEY_UP)
				{
					if (y != 2)	y -= 2;
				}
				else if (a == KEY_DOWN)
				{
					if (y != ls.SizeColumns * 2) y += 2;
				}
				else if (a == KEY_RIGHT)
				{
					if (x != ((ls.SizeRows - 1) * 4) + 2) x += 4;
				}
				else if (a == KEY_LEFT)
				{
					if (x != 2)	x -= 4;
				}
			}
			gotoXY(x, y);
		}

		if (CheckWon() && !solvedbycode)
		{
			gotoXY(0, ls.SizeColumns * 2 + 2);
			cout << "You have won !" << endl;
			system("PAUSE");
			MainMenu();
		}
		else if (solvedbycode)
		{
			gotoXY(0, ls.SizeColumns * 2 + 2);
			cout << "Game solved successfully." << endl;
			system("PAUSE");
			MainMenu();
		}
	}

	void gotoXY(float eex, float eey)
	{
		COORD coord;
		coord.X = eex;
		coord.Y = eey;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	void MainMenu()
	{
		system("CLS");
		int choice;
		cout << "\n\t\t\t\t=======================================\n";
		cout << "\t\t\t\t|| (1) | New Game.                   ||\n";
		cout << "\t\t\t\t||-----|-----------------------------||\n";
		cout << "\t\t\t\t|| (2) | Load Game.                  ||\n";
		cout << "\t\t\t\t||-----|-----------------------------||\n";
		cout << "\t\t\t\t|| (3) | Solve your game.            ||\n";
		cout << "\t\t\t\t||-----|-----------------------------||\n";
		cout << "\t\t\t\t|| (0) | Exit.                       ||\n";
		cout << "\t\t\t\t=======================================\n";
		cout << "\t\t\t\t\t\tChoice: ";	cin >> choice;
		while (1)
		{
			if (cin.fail() || choice < 0)
			{
				cin.clear();	cin.ignore(256, '\n');
				cout << "\t\t\t\tInvalid input. Enter a number: ";		cin >> choice;
				continue;
			}
			break;
		}

		switch (choice)
		{
		case 1:
			DiffucltyMenu();	break;
		case 2:
			ViewSavedGames();	break;
		case 3:
			SolveYourGame();	break;
		case 0:
			exit(0);	 break;
		default:
			MainMenu(); break;
		}
	}

	void DiffucltyMenu()
	{
		int choice;
		cout << "\n\n\t\t1. Easy level.\t\t2. Medium level.\t\t3. Hard level.\t\t0. Main menu.\n\n\t\t\t\t\t\tChoice: ";	cin >> choice;
		while (1)
		{
			if (cin.fail() || choice < 0)
			{
				cin.clear();	cin.ignore(256, '\n');
				cout << "Invalid input. Enter a number: ";		cin >> choice;
				continue;
			}
			break;
		}
		if (choice == 1 || choice == 2 || choice == 3)
			StartGame(choice);
		else if (choice == 0)
			MainMenu();
		else
		{
			cout << "\t\t\t\t\t\tInvalid input.\n\t\t\t\t    Press any key to return to main menu...";
			_getche();
			MainMenu();
		}
	}

	void SolveYourGame()
	{
		ls.Clear();
		system("CLS");
		PlayerMove pm;
		pm.CurrentValue = 0;
		pm.PreviousValue = 0;
		pm.Location.Row = 0;
		pm.Location.Column = 0;
		float x = 2, y = 2;
		GameDisplay();
		gotoXY(x, y);

		int a = _getch();

		while (a != KEY_ESC && !CheckWon())
		{
			if (_kbhit())
			{
				a = _getch();
				if (a == KEY_C)
				{
					if (SolveSoduko())
					{
						system("CLS");
						gotoXY(0, 0);
						GameDisplay();
						gotoXY(0, ls.SizeColumns * 2 + 2);
						cout << "Game solved succefully." << endl;
						system("PAUSE");
						MainMenu();
					}
					else
					{
						gotoXY(0, ls.SizeColumns * 2 + 2);
						cout << "No solution found." << endl;
						system("PAUSE");
						MainMenu();
					}
				}
				else if (a == KEY_H)
				{
					Position p;
					p.Row = y / 2;
					p.Column = ((x - 2) / 4) + 1;
					int value = fg.GetNodeAtPosition(p.Row, p.Column)->data;
					if (!PositionExists(RandomFinalPositions, TotalFilledPositions, p))
					{
						ls.SetNodeValueAtPosition(p.Row, p.Column, value);
						gotoXY(0, 0);
						GameDisplay();
						gotoXY(x, y);
					}
				}
				else if (a == KEY_U)
				{
					Position p;
					p.Row = y / 2;
					p.Column = ((x - 2) / 4) + 1;
					if (!PositionExists(RandomFinalPositions, TotalFilledPositions, p))
					{
						if (pm.Location.Row != 0 && pm.Location.Column != 0)
						{
							ls.SetNodeValueAtPosition(pm.Location.Row, pm.Location.Column, pm.PreviousValue);
							gotoXY(0, 0);
							GameDisplay();
							gotoXY(x, y);
						}
					}
				}
				else if (a == KEY_R)
				{
					Position p;
					p.Row = y / 2;
					p.Column = ((x - 2) / 4) + 1;
					if (!PositionExists(RandomFinalPositions, TotalFilledPositions, p))
					{
						if (pm.Location.Row != 0 && pm.Location.Column != 0)
						{
							ls.SetNodeValueAtPosition(pm.Location.Row, pm.Location.Column, pm.CurrentValue);
							gotoXY(0, 0);
							GameDisplay();
							gotoXY(x, y);
						}
					}
				}
				else if (a == KEY_B || a == KEY_BACK)
				{
					gotoXY(0, 0);
					MainMenu();
				}
				else if (a == KEY_NUMPAD0 || a == KEY_NUMPAD1 || a == KEY_NUMPAD2 || a == KEY_NUMPAD3 || a == KEY_NUMPAD4 || a == KEY_NUMPAD5 || a == KEY_NUMPAD6 || a == KEY_NUMPAD7 || a == KEY_NUMPAD8 || a == KEY_NUMPAD9)
				{
					Position p;
					p.Row = y / 2;
					p.Column = ((x - 2) / 4) + 1;
					if (a != KEY_NUMPAD0)
					{
						if (ls.ValidLocation(p.Row, p.Column, a - 48))
						{
							pm.PreviousValue = ls.GetNodeAtPosition(p.Row, p.Column)->data;
							ls.SetNodeValueAtPosition(p.Row, p.Column, a - 48);
							pm.CurrentValue = a - 48;
							pm.Location = p;
							gotoXY(0, 0);
							GameDisplay();
							gotoXY(0, ls.SizeColumns * 2 + 2);
							cout << "                                      " << endl;
							gotoXY(x, y);
						}
						else
						{
							gotoXY(0, ls.SizeColumns * 2 + 2);
							cout << "Invalid location." << endl;
							gotoXY(x, y);
						}
					}
					else
					{
						pm.PreviousValue = ls.GetNodeAtPosition(p.Row, p.Column)->data;
						ls.SetNodeValueAtPosition(p.Row, p.Column, a - 48);
						pm.CurrentValue = a - 48;
						pm.Location = p;
						gotoXY(0, 0);
						GameDisplay();
						gotoXY(0, ls.SizeColumns * 2 + 2);
						cout << "                                      " << endl;
						gotoXY(x, y);
					}
				}
				else if (a == KEY_UP)
				{
					if (y != 2)	y -= 2;
				}
				else if (a == KEY_DOWN)
				{
					if (y != ls.SizeColumns * 2) y += 2;
				}
				else if (a == KEY_RIGHT)
				{
					if (x != ((ls.SizeRows - 1) * 4) + 2) x += 4;
				}
				else if (a == KEY_LEFT)
				{
					if (x != 2)	x -= 4;
				}
			}

			gotoXY(x, y);
		}
	}

	void SaveProgress(string FileName)
	{
		int arr[81], counter = 0, solved = 0;
		float progress = 0;
		ofstream delout("Saves\\" + FileName + ".txt");
		delout.clear();	delout.close();
		Node* RowCurrent = ls.GetHead();
		Node* ColCurrent = RowCurrent->right;
		for (int i = 1; i <= ls.SizeRows; i++)
		{
			if (RowCurrent->data != 0)
			{
				arr[counter] = RowCurrent->data;
				solved++;
				counter++;
			}
			else
			{
				arr[counter] = 0;
				counter++;
			}

			for (int j = 2; j <= ls.SizeColumns; j++)
			{
				if (ColCurrent->data != 0)
				{
					arr[counter] = ColCurrent->data;
					counter++;
					solved++;
				}
				else
				{
					arr[counter] = 0;
					counter++;
				}

				if (ColCurrent->right != NULL)
				{
					ColCurrent = ColCurrent->right;
				}
			}
			if (RowCurrent->down != NULL)
			{
				RowCurrent = RowCurrent->down;
				ColCurrent = RowCurrent->right;
			}
		}
		ofstream out("Saves\\" + FileName + ".txt", ios::app);
		float x = float(solved) / float(81);
		progress = x * 100;
		out << GameDifficulty << endl;
		out << progress << endl;
		for (int i = 0; i < counter; i++)
		{
			out << arr[i] << endl;
		}

		out.close();

		cout << "Progress Saved Successfully." << endl;
	}

	void ViewSavedGames()
	{
		string path = "Saves";
		SavesCount = 0;

		for (const auto& entry : fs::directory_iterator(path))
		{
			SavesNames[SavesCount] = entry.path().filename().string();
			SavesCount++;
		}

		for (int i = 0; i < SavesCount; i++)
		{
			ifstream inn;
			inn.open("Saves\\" + SavesNames[i], ios::in);
			string val = "";
			int count = 0;
			sg[i].ID = i + 1;
			sg[i].Name = SavesNames[i];
			getline(inn, val);
			sg[i].Difficulty = val;
			getline(inn, val);
			sg[i].Progress = val;
		}

		for (int i = 0; i < SavesCount; i++)
		{
			cout << sg[i].ID << ". Name : " << sg[i].Name << endl << "Difficulty : " << sg[i].Difficulty << endl << "Progress : " << sg[i].Progress << " %" << endl << "-------------------------" << endl;
		}

		int choice;
		cout << "Enter Save number or 0 to Back : ";		cin >> choice;
		while (1)
		{
			if (cin.fail() || choice < 0 || choice > SavesCount)
			{
				cin.clear();	cin.ignore(256, '\n');
				cout << "\t\t\t\tInvalid input. Enter a number: ";		cin >> choice;
				continue;
			}
			break;
		}
		if (choice != 0)
		{
			LoadGameFromFile(sg[choice - 1].Name);
			StartGame(0);
		}
		else
		{
			MainMenu();
		}
	}

	void LoadGameFromFile(string FileName)
	{
		int arr[256];
		ifstream in("Saves\\" + FileName);

		string str = "";
		getline(in, str);
		getline(in, str);
		for (int i = 0; i < 81; i++)
		{
			in >> arr[i];
		}
		int count = 0;

		for (int j = 1; j <= 9; j++)
		{
			for (int i = 1; i <= 9; i++)
			{
				ls.SetNodeValueAtPosition(j, i, arr[count]);
				count++;
			}
		}
		in.close();
	}
};

int main()
{
	system("color 3F");
	SodukoGame sg = SodukoGame(9, 9);
	sg.MainMenu();

	return 0;
}

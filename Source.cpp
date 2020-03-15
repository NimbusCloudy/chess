
#include <iostream>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <fstream>
using namespace std;
#define MAX 10
#define MROWS 80
#define MCOLS 80
#define WhitePiece 0
#define BlackPiece 1

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}

void gotorowcol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void Color(int k)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);
}

bool IsVerticalMove(int sc, int ec)
{
	if (sc == ec)
		return true;
	else
		return false;
}

bool IsHorizontalMove(int sr, int er)
{
	if (sr == er)
		return true;
	else
		return false;
}

bool IsDiagonalMove(int sr, int sc, int er, int ec)
{
	int Dr = er - sr;
	int Dc = ec - sc;
	if (Dr == Dc || Dr == -Dc)
		return true;
	else
		return false;
}

bool IsVerticalPathClear(char B[][MAX], int sr, int sc, int er, int ec)
{
	if (sr < er)
	{
		for (int r = sr + 1; r < er; r++)
		{
			if (B[r][sc] != '-')
				return false;
		}
		return true;
	}
	else
	{
		for (int r = sr - 1; r > er; r--)
		{
			if (B[r][sc] != '-')
				return false;
		}
		return true;
	}
}

bool IsHorizontalPathClear(char B[][MAX], int sr, int sc, int er, int ec)
{
	if (sc < ec)
	{
		for (int c = sc + 1; c < ec; c++)
		{
			if (B[sr][c] != '-')
				return false;
		}
		return true;
	}
	else
	{
		for (int c = sc - 1; c > ec; c--)
		{
			if (B[sr][c] != '-')
				return false;
		}
		return true;
	}
}

bool IsDiagonalPathClear(char B[][MAX], int sr, int sc, int er, int ec)
{
	int Dr = er - sr;
	int Dc = ec - sc;
	int c = sc + 1;
	if (Dr > 0 && Dc > 0)
	{
		for (int r = sr + 1; r < er; r++)
		{
			if (B[r][c] != '-')
			{
				return false;
			}
			c++;
		}
	}
	c = sc - 1;
	if (Dr < 0 && Dc < 0)
	{
		for (int r = sr - 1; r > er; r--)
		{
			if (B[r][c] != '-')
			{
				return false;
			}
			c--;
		}
	}
	c = sc - 1;
	if (Dr > 0 && Dc < 0)
	{
		for (int r = sr + 1; r < er; r++)
		{
			if (B[r][c] != '-')
			{
				return false;
			}
			c--;
		}
	}
	c = sc + 1;
	if (Dr < 0 && Dc >0)
	{
		for (int r = sr - 1; r > er; r--)
		{
			if (B[r][c] != '-')
			{
				return false;
			}
			c++;
		}
	}
}

bool IsRookLegalMove(char B[][MAX], int sr, int sc, int er, int ec)
{
	return IsVerticalMove(sc, ec) && IsVerticalPathClear(B, sr, sc, er, ec)
		||
		IsHorizontalMove(sr, er) && IsHorizontalPathClear(B, sr, sc, er, ec);
}

bool IsHorseLegalMove(char B[][MAX], int sr, int sc, int er, int ec)
{
	if (er == sr + 1 && ec == sc + 2)
		return true;
	if (er == sr - 1 && ec == sc + 2)
		return true;
	if (er == sr - 2 && ec == sc + 1)
		return true;
	if (er == sr - 2 && ec == sc - 1)
		return true;
	if (er == sr - 1 && ec == sc - 2)
		return true;
	if (er == sr + 1 && ec == sc - 2)
		return true;
	if (er == sr + 2 && ec == sc - 1)
		return true;
	if (er == sr + 2 && ec == sc + 1)
		return true;
	else
		return false;
}

bool IsBishopLegalMove(char B[][MAX], int sr, int sc, int er, int ec)
{
	return IsDiagonalMove(sr, sc, er, ec) && IsDiagonalPathClear(B, sr, sc, er, ec);
}

bool IsKingLegalMove(char B[][MAX], int sr, int sc, int er, int ec)
{
	if (IsVerticalPathClear(B, sr, sc, er, ec) == false || IsHorizontalPathClear(B, sr, sc, er, sc) == false)
	{
		return false;
	}
	if ((er == sr + 1 && ec == sc - 1) || (er == sr + 1 && ec == sc + 1)    // Right Diag || Left Diag	
		|| (er == sr - 1 && ec == sc - 1) || (er == sr - 1 && ec == sc + 1) // Right Diag Down || Left Diag Up
		|| (er == sr + 1 && ec == sc) || (er == sr - 1 && ec == sc)         // Ahead 
		|| (er == sr && ec == sc + 1) || (er == sr && ec == sc - 1))        //
	{
		return true;
	}
	else
		return false;
}

bool IsQueenLegalMove(char B[][MAX], int sr, int sc, int er, int ec)
{
	return (IsVerticalMove(sc, ec) && IsVerticalPathClear(B, sr, sc, er, ec))
		|| (IsHorizontalMove(sr, er) && IsHorizontalPathClear(B, sr, sc, er, ec))
		|| (IsDiagonalMove(sr, sc, er, ec) && IsDiagonalPathClear(B, sr, sc, er, ec));
}

bool IsSmall(char S)
{
	return S >= 'a' && S <= 'z';
}

bool IsCapital(char S)
{
	if (S >= 'A' && S <= 'Z')
		return true;
	else
		return false;
}

bool IsMyChessPiece(char S, int turn)
{
	if (turn == WhitePiece)
		return IsCapital(S);
	return IsSmall(S);
}

bool IsPawnKillValid(char B[][MAX], int er, int ec, int turn)
{
	if (turn == WhitePiece)
	{
		if (B[er][ec] >= 'a' && B[er][ec] <= 'z')
			return true;
		return false;
	}
	if (turn == BlackPiece)
	{
		if (B[er][ec] >= 'A' && B[er][ec] <= 'Z')
			return true;
		return false;
	}
}

bool IsPawnLegalMove(char B[][MAX], int sr, int sc, int er, int ec, int turn)
{
	if (sr == 2 && turn == BlackPiece && IsVerticalPathClear(B, sr, sc, er, ec) == true && (B[er][ec] == '-'))  //First Turn Check
	{
		if ((er - sr == 2 || er - sr == 1) && ec == sc)
			return true;
	}
	if (sr == 7 && turn == WhitePiece && IsVerticalPathClear(B, sr, sc, er, ec) == true && (B[er][ec] == '-')) //First Turn Check
	{
		if ((er - sr == -2 || er - sr == -1) && ec == sc)
			return true;
	}
	if (B[er][ec] == '-')
	{
		if (turn == WhitePiece && (er == sr - 1) && (ec == sc))
			return true;
		if (turn == BlackPiece && (er == sr + 1) && (ec == sc))
			return true;
	}
	if (IsPawnKillValid(B, er, ec, turn) == true)  // Pawn Kill // IsMyChessPiece was checked in IsValidDestination.
	{
		if (turn == WhitePiece && (er == sr - 1 && (ec == sc - 1 || ec == sc + 1)))
			return true;
		if (turn == BlackPiece && (er == sr + 1 && (ec == sc - 1 || ec == sc + 1)))
			return true;
	}
	return false;
}

bool IsValidMove(char B[][MAX], int sr, int sc, int er, int ec, int turn)
{
	switch (B[sr][sc])
	{
	case 'R':
	case 'r':
		return IsRookLegalMove(B, sr, sc, er, ec);
	case 'H':
	case 'h':
		return IsHorseLegalMove(B, sr, sc, er, ec);
	case 'B':
	case 'b':
		return IsBishopLegalMove(B, sr, sc, er, ec);
	case 'Q':
	case 'q':
		return IsQueenLegalMove(B, sr, sc, er, ec);
	case 'K':
	case 'k':
		return IsKingLegalMove(B, sr, sc, er, ec);
	case 'P':
	case 'p':
		return IsPawnLegalMove(B, sr, sc, er, ec, turn);
	}
}

void PlayerMsg(int turn)
{
	Color(15);
	gotorowcol(6, 70);
	if (turn == WhitePiece)
		cout << "White's Turn ";
	else
		cout << "Black's Turn";
}

void PrintBox(int sr, int sc, int rdim, int cdim, char s, int C)
{
	Color(C);
	for (int r = 0; r < rdim; r++)
	{
		for (int c = 0; c < rdim; c++)
		{
			if (r == 0 || r == rdim - 1 || c == 0 || c == cdim - 1)
			{
				gotorowcol(sr + r, sc + c);
				cout << s;
			}
		}
	}
}

void PrintGrid(int rows, int clns, int d, char s, int rdim, int cdim)
{
	int C = 8;
	for (int r = 1; r <= d; r++)
	{
		for (int c = 1; c <= d; c++)
		{
			if (r % 2 != 0 && c % 2 == 0 || c % 2 != 0 && r % 2 == 0)
				C = 15;
			else
				C = 8;
			int sr = r * rdim, sc = c * cdim;
			PrintBox(sr, sc, rdim, cdim, s, C);
		}
	}
}

bool IsValidSelection(char B[][MAX], int turn, int sr, int sc)
{
	if (sr < 0 || sr > MAX || sc < 0 || sc > MAX)
		return false;
	return IsMyChessPiece(B[sr][sc], turn);
}

bool IsValidDestination(char B[][MAX], int turn, int er, int ec)
{
	if (er < 1 || er >= MAX - 1 || ec < 1 || ec >= MAX - 1)
		return false;
	if (!IsMyChessPiece(B[er][ec], turn))
		return true;
	else
		return false;
}

void HighLight(char HB[][MAX], int sr, int sc, char B[][MAX], int turn)
{
	for (int ri = 0; ri < MAX; ri++)
		for (int ci = 0; ci < MAX; ci++)
			HB[ri][ci] = B[ri][ci];
	for (int er = 1; er < 9; er++)
	{
		for (int ec = 1; ec < 9; ec++)
		{
			if (IsValidDestination(B, turn, er, ec) && IsValidMove(B, sr, sc, er, ec, turn))
				HB[er][ec] = '*';
		}
	}
}

void Move(char B[][MAX], int sr, int sc, int er, int ec)
{
	B[er][ec] = B[sr][sc];
	B[sr][sc] = '-';
}

int ToUpper(char c)
{
	if (c >= 'a' && c <= 'z')
		c = c - 32;
	return c;
}

void TurnChange(int& turn)
{
	turn++;
	if (turn == 2)
		turn = 0;
}

void Load(ifstream& reader, char B[][MAX], int DIM)
{
	for (int ri = 0; ri < MAX; ri++)
	{
		for (int ci = 0; ci < MAX; ci++)
		{
			reader >> B[ri][ci];
		}
	}
}

void ClearMsg()
{
	for (int r = 0; r < 40; r++)
	{
		gotorowcol(6 + r, 70);
		for (int c = 0; c < 45; c++)
		{
			cout << ' ';
		}
	}
}

bool IsCastlingPieceMoved(char B[][MAX], int turn, bool& RWR, bool& LWR, bool& RBR, bool& LBR, bool& WK, bool& BK, int sr, int sc, int er, int ec)
{
	bool TRWR = false, TLWR = false, TRBR = false, TLBR = false, TWK = false, TBK = false;
	if (turn == WhitePiece)
	{
		if ((sr == 8 && sc == 5) || (er != 8 && ec != 7)) // WHITE KING && RIGHT ROOK 
		{
			TWK = true; TRWR = true; TLWR = true;
		}
		if ((sr == 8 && sc == 5) || (er != 8 && ec != 3)) // WHITE KING && LEFT ROOK 
		{
			TWK = true; TRWR = true; TLWR = true;
		}
		if ((sr == 8 && sc == 1) && (er == 8 && ec == 4))  // RIGHT ROOK
		{
			TRWR = true;
		}
		else if ((sr == 8 && sc == 8) && (er == 8 && ec == 6))  // LEFT ROOK
		{
			TLWR = true;
		}
		if ((sr == 8 && sc == 5) && WK == true)
			return true;
		if ((sr == 8 && sc == 1) && RWR == true)
			return true;
		if ((sr == 8 && sc == 8) && LWR == true)
			return true;
		if (TWK == true)
		{
			WK = true; RWR = true; LWR = true;
		}
		if (TLWR == true)
			LWR = true;
		if (RWR == true)
			TRWR = true;
		return false;
	}
	if (turn == BlackPiece)
	{
		if ((sr == 1 && sc == 4) || (er != 1 && ec != 2)) // Black KING && LEFT ROOK 
		{
			TBK = true; TRBR = true; TLBR = true;
		}
		if ((sr == 1 && sc == 4) || (er != 1 && ec != 2)) // Black KING && RIGHT ROOK 
		{
			TBK = true; TRBR = true; TLBR = true;
		}
		if ((sr == 1 && sc == 1) && (er == 1 && ec == 3))      // RIGHT ROOK
		{
			TRBR = true;
		}
		else if ((sr == 1 && sc == 8) && (er == 1 && ec == 5))  // LEFT ROOK
		{
			TLBR = true;
		}
		if ((sr == 1 && sc == 4) && BK == true)
			return true;
		if ((sr == 1 && sc == 1) && RBR == true)
			return true;
		if ((sr == 1 && sc == 8) && LBR == true)
			return true;
		if (TBK == true)
		{
			BK = true; RBR = true; LBR = true;
		}
		if (TLBR == true)
			LBR = true;
		if (RBR == true)
			TRBR = true;
		return false;
	}

}

bool CastlingPathClear(char B[][MAX], int turn, int sr, int sc, int er, int ec)
{
	if (turn == WhitePiece)
	{
		if ((sr == 8 && sc == 5) && (er == 8 && ec == 7)) // WHITE KING && RIGHT ROOK 
			if (B[8][7] == '-' && B[8][6] == '-')
				return true;
		if ((sr == 8 && sc == 5) && (er == 8 && ec == 3))  // WHITE KING  and Left ROOK
			if (B[8][2] == '-' && B[8][3] == '-' && B[8][4] == '-')
				return true;
		if ((sr == 8 && sc == 1) && (er == 8 && ec == 4))  // LEFT ROOK
			if (B[8][2] == '-' && B[8][3] == '-' && B[8][4] == '-')
				return true;
		if ((sr == 8 && sc == 8) && (er == 8 && ec == 6))  // RIGHT ROOK
			if (B[8][7] == '-'&& B[8][6] == '-')
				return true;
	}
	if (turn == BlackPiece)
	{
		if ((sr == 1 && sc == 4) && (er == 1 && ec == 2)) // BLACK KING && RIGHT ROOK 
			if (B[1][2] == '-' && B[1][3] == '-')
				return true;
		if ((sr == 1 && sc == 4) && (er == 1 && ec == 6))  // BLACK KING  and Left ROOK
			if (B[1][7] == '-' && B[1][6] == '-'&&  B[1][5] == '-')
				return true;
		if ((sr == 1 && sc == 1) && (er == 1 && ec == 3))  // RIGHT ROOK
			if (B[1][2] == '-' && B[1][3] == '-' )
				return true;
		if ((sr == 1 && sc == 8) && (er == 1 && ec == 5))  // LEFT ROOK
			if (B[1][7] == '-' && B[1][6] == '-' && B[1][5] == '-')
				return true;
	}
}

void Castling(char B[][MAX], int turn, bool& RWR, bool& LWR, bool& RBR, bool& LBR, bool& WK, bool& BK, int sr, int sc, int er, int ec, bool& CastlingHappen)
{
	if (IsCastlingPieceMoved(B, turn, RWR, LWR, RBR, LBR, WK, BK, sr, sc, er, ec) == false)
	{
		if (CastlingPathClear(B, turn, sr, sc, er, ec) == true)
		{
			if (turn == WhitePiece)
			{
				if ((sr == 8 && sc == 5) && (er == 8 && ec == 7) || ((sr == 8 && sc == 8) && (er == 8 && ec == 6)))  // WHITE KING
				{
					B[8][6] = B[8][8]; B[8][7] = B[8][5]; B[8][5] = '-'; B[8][8] = '-';
					CastlingHappen = true;
				}
				else if ((sr == 8 && sc == 5) && (er == 8 && ec == 3) || ((sr == 8 && sc == 1) && (er == 8 && ec == 4)))  // WHITE KING
				{
					B[8][3] = B[8][5]; B[8][4] = B[8][1]; B[8][5] = '-'; B[8][1] = '-';
					CastlingHappen = true;
				}
			}
			if (turn == BlackPiece)
			{
				if ((sr == 1 && sc == 4) && (er == 1 && ec == 2) || ((sr == 1 && sc == 1) && (er == 1 && ec == 3)))  // WHITE KING
				{
					B[1][2] = B[1][4]; B[1][3] = B[1][1]; B[1][1] = '-'; B[1][4] = '-';
					CastlingHappen = true;
				}
				else if ((sr == 1 && sc == 4) && (er == 1 && ec == 6) || ((sr == 1 && sc == 8) && (er == 1 && ec == 5)))  // WHITE KING
				{
					B[1][5] = B[1][8];	B[1][6] = B[1][4]; B[1][4] = '-'; B[1][8] = '-';
					CastlingHappen = true;
				}
			}
		}

	}
}

void MakeArrayCopy(char B[][MAX], char Temp[][MAX])
{
	for (int r = 0; r < MAX; r++)
		for (int c = 0; c < MAX; c++)
			Temp[r][c] = B[r][c];
}

void SaveAndExit(char B[][MAX], int turn)
{
	ofstream wtr("load.txt");
	for (int ri = 0; ri < MAX; ri++)
		for (int ci = 0; ci < MAX; ci++)
		{
			wtr << B[ri][ci];
		}
	wtr << turn;
}

void Save(char B[][MAX], char Save[][500], int SaveNo)
{
	for (int ri = 0; ri < MAX; ri++)
		for (int ci = 0; ci < MAX; ci++)
		{
			Save[ri + SaveNo][ci + SaveNo] = B[ri][ci];
		}
}

void LoadSavedData(char B[][MAX], int& turn)
{
	ifstream rdr("load.txt");
	for (int ri = 0; ri < MAX; ri++)
		for (int ci = 0; ci < MAX; ci++)
		{
			rdr >> B[ri][ci];
		}
	rdr >> turn;
}

void Init(char B[][MAX], int& turn, int d)
{
	ifstream rdr("chess.txt");
	Load(rdr, B, d);
	turn = 1;
}

void DisplayCheck(int turn)
{
	Color(4);
	gotorowcol(10, 70);
	if (turn == WhitePiece)
		cout << "CHECK TO BLACK";
	else
		cout << "CHECK TO WHITE";
	Color(15);
}

void PawnPromotion(char B[][MAX], int turn, int sr, int sc, int er, int ec, bool& IsPawnPromoted)
{
	ClearMsg();
	char piece = ' ';
	if (turn == WhitePiece && B[sr][sc] == 'P' && (er == 1) && (ec >= 1 && ec <= 8))
	{
		gotorowcol(10, 70);
		cout << "Which Piece You Want ?";
		while ((piece != 'Q') && (piece != 'R') && (piece != 'H') && (piece != 'B'))
		{
			cin >> piece;
		}
		B[sr][sc] = '-';
		B[er][ec] = piece;
		IsPawnPromoted = true;
	}
	if (turn == BlackPiece && B[sr][sc] == 'p' && (er == 8) && (ec >= 1 && ec <= 8))
	{
		gotorowcol(10, 70);
		cout << "Which Piece You Want ?";
		while ((piece != 'q') && (piece != 'r') && (piece != 'h') && (piece != 'b'))
		{
			cin >> piece;
		}
		B[sr][sc] = '-';
		B[er][ec] = piece;
		IsPawnPromoted = true;
	}
	ClearMsg();
}

void Undo(char B[][MAX], char temp[][MAX], bool& UndoHappen)
{
	int r, c;
	PrintBox(20, 70, 10, 10, -37, 10);
	Color(10);
	gotorowcol(25, 73);
	cout << "UNDO";
	gotorowcol(31, 71);
	cout << "CLICK ANYWHERE ELSE THAN THE BOX TO NOT UNDO";
	getRowColbyLeftClick(r, c);
	ClearMsg();
	if (r >= 20 && r <= 30 && c >= 70 && c <= 80)
	{
		MakeArrayCopy(temp, B);
		UndoHappen = true;
	}
	ClearMsg();
	Color(15);
}

void Options(bool& New, bool& Load)
{
	Color(15);
	int r = 0, c = 0;
	gotorowcol(1, 10);
	cout << "C H E S S\n\n";
	gotorowcol(5, 5);
	cout << "N : NEW GAME\n";
	gotorowcol(7, 5);
	cout << "----------------------";
	gotorowcol(10, 5);
	cout << "L : LOAD GAME\n";
	getRowColbyLeftClick(r, c);
	gotorowcol(0, 0);
	for (int r = 0; r < 11; r++)
	{
		for (int c = 10; c < 100; c++)
		{
			cout << ' ';
		}
		cout << endl;
	}
	if (r < 7)
		New = true;
	if (r > 7)
		Load = true;
}

void PieceToMove(int& r, int& c, int col, int rdim, int  cdim)
{
	int sr, sc;
	do
	{
		Color(15);
		gotorowcol(8, 70);
		cout << "SELECT PIECE";
		getRowColbyLeftClick(sr, sc);
	} while (sr > 52 || sr < 7 || sc > 52 || sc < 7);
	ClearMsg();
	r = sr / rdim;
	c = sc / cdim;
	gotorowcol(10, 70);
	cout << " SR :" << r << " SC :" << c;
}

void AskForDestination(int& r, int& c, int col, int rdim, int cdim)
{
	Color(15);
	int er, ec;
	do
	{
		gotorowcol(14, 70);
		cout << "SELECT DESTINATION";
		getRowColbyLeftClick(er, ec);
		if (er > 52 || er < 7 || ec > 52 || ec < 7)
		{
			gotorowcol(59, 70);
			Color(4);
			cout << "INVALID DESTINATION";
			Color(15);
		}
	} while (er > 52 || er < 7 || ec > 52 || ec < 7);
	r = er / rdim;
	c = ec / cdim;
	gotorowcol(16, 70);
	cout << " ER :" << r << " EC :" << c;
}

void FindKing(char B[][MAX], int turn, int& kr, int& kc)
{
	for (int r = 1; r < MAX ; r++)
	{
		for (int c = 1; c < MAX ; c++)
		{
			if (IsMyChessPiece(B[r][c], turn) && (B[r][c] == 'k' || B[r][c] == 'K'))
			{
				kr = r; kc = c;
				return;
			}
		}
	}
}

bool Check(char B[][MAX], int turn)
{
	int kr = 0, kc = 0;
	TurnChange(turn);
	FindKing(B, turn, kr, kc);
	TurnChange(turn);
	for (int sr = 1; sr < MAX - 1; sr++)
		for (int sc = 1; sc < MAX - 1; sc++)
			if (IsMyChessPiece(B[sr][sc], turn) && IsValidMove(B, sr, sc, kr, kc, turn))
				return true;
	return false;
}

void DisplayBoard(char B[][MAX], int rdim, int turn)
{
	for (int ri = 0; ri < MAX; ri++)
	{
		for (int ci = 0; ci < MAX; ci++)
		{
			gotorowcol(rdim * ri + rdim / 2, rdim * ci + rdim / 2);
			if (B[ri][ci] != 'z')
			{
				if (B[ri][ci] == '*')
					Color(4);
				if (B[ri][ci] >= 'A' && B[ri][ci] <= 'Z')
				{
					if (Check(B, turn) && turn == BlackPiece && B[ri][ci] == 'K')
					{
						Color(4);
					}
					else
						Color(15);
				}
				if (B[ri][ci] >= 'a' && B[ri][ci] <= 'z')
				{
					if (Check(B, turn) && turn == WhitePiece && B[ri][ci] == 'k')
					{
						Color(4);
					}
					else
						Color(8);
				}
				if (ri == 0 || ri == 9 || ci == 0 || ci == 9)
					Color(10);
				cout << B[ri][ci];
				Color(8);
			}
		}
	}
}

bool SelfCheck(char B[][MAX], int sr, int sc, int er, int ec, int turn)
{
	int kr = 0, kc = 0;
	char temp[MAX][MAX];
	MakeArrayCopy(B, temp);
	Move(temp, sr, sc, er, ec);
	FindKing(temp, turn, kr, kc);
	TurnChange(turn);
	for (int r = 1; r < MAX - 1; r++)
	{
		for (int c = 1; c < MAX - 1; c++)
 	{
			if (IsMyChessPiece(temp[r][c], turn) && IsValidMove(temp, r, c, kr, kc, turn))
			{
				return true;
				gotorowcol(90, 90);
				cout <<"R" << r <<"C" <<c;
			}
		}
	}
	return false;
}

bool CheckMate(char B[][MAX], int turn)
{
	if (Check(B, turn) == true) // black checked white // comments for own understanding //
	{
		TurnChange(turn); // turn white
		for (int sr = 1; sr < MAX - 1; sr++)
		{
			for (int sc = 1; sc < MAX - 1; sc++)
			{
				if (IsMyChessPiece(B[sr][sc], turn)) // My Piece Confirmed   
				{
					for (int er = 1; er < MAX - 1; er++)
					{
						for (int ec = 1; ec < MAX - 1; ec++)
						{
							if (!IsMyChessPiece(B[er][ec], turn) && IsValidMove(B, sr, sc, er, ec, turn))
							{
								if (SelfCheck(B, sr, sc, er, ec, turn) == false)
								{
									return false;
								}
							}
						}
					}
				}
			}
		}
		return true;
	}
}

bool StaleMate(char B[][MAX], int turn)
{
	for (int sr = 1; sr < MAX - 1; sr++)
	{
		for (int sc = 1; sc < MAX - 1; sc++)
		{
			if (IsMyChessPiece(B[sr][sc], turn)) // My Piece Confirmed   
			{
				for (int er = 1; er < MAX - 1; er++)
				{
					for (int ec = 1; ec < MAX - 1; ec++)
					{
						if (!IsMyChessPiece(B[er][ec], turn) && IsValidMove(B, sr, sc, er, ec, turn))
						{
							if (SelfCheck(B, sr, sc, er, ec, turn) == false)
								return false;
						}
					}
				}
			}
		}
	}
	return true;
}

void DisplayWinOrDraw(bool checkmate, bool stalemate, bool TimerWin, int  turn)
{
	Color(4);
	gotorowcol(18, 70);
	if (TimerWin == true)
	{
		if (turn == WhitePiece)
			cout << "WHITE LOST BY TIME";
		else
			cout << "BLACK LOST BY TIME";
	}
	if (checkmate == true)
	{

		if (turn == WhitePiece)
			cout << "WHITE WON THE GAME";
		else
			cout << "BLACK WON THE GAME";
	}
	Color(10);
	if (stalemate == true)
		cout << "STALE MATE...";
}

void DisplayeReplay(char B[][500], int rdim, int savedno)
{
	for (int ri = savedno; ri < MAX; ri++)
	{
		for (int ci = 0; savedno < MAX; ci++)
		{
			gotorowcol(rdim * ri + rdim / 2, rdim * ci + rdim / 2);
			if (B[ri][ci] != 'z')
			{
				if (B[ri][ci] >= 'A' && B[ri][ci] <= 'Z')
				{
					Color(4);
				}
				if (B[ri][ci] >= 'a' && B[ri][ci] <= 'z')
				{
					Color(8);
				}
				if (ri == 0 || ri == 9 || ci == 0 || ci == 9)
					Color(10);
				cout << B[ri + savedno][ci + savedno];
				Color(15);
			}
		}
	}
}

void SaveGame(ofstream& save, char B[][MAX])
{
	for (int r = 0; r < MAX; r++)
	{
		for (int c = 0; c < MAX; c++)
		{
			save << B[r][c];
		}
	}
}

int main()
{
	srand(time(0));
	char B[MAX][MAX];     // for Board
	char HB[MAX][MAX];    // for highlight
	char Temp[MAX][MAX];  // for undo
	int turn = 0, sr, sc, er, ec, ROW = 80, COL = 80, dim = 10;
	int rdim = ROW / (10 + 2), cdim = COL / (10 + 2);
	char S = -37;
	bool RWR = false, LWR = false, RBR = false, LBR = false, WK = false, BK = false;    // Castling
	bool  CastlingHappen = false;     // Castling
	bool  IsPawnPromoted = false;
	bool NewFile = false, LoadFile = false;
	bool UndoHappen = false;
	bool TimerWin = false, checkmate = false, stalemate = false;
	ofstream save("SAVE.txt");
	while ((NewFile == false && LoadFile == false))
		Options(NewFile, LoadFile);
	if (NewFile == true)
		Init(B, turn, MAX);
	if (LoadFile == true)
		LoadSavedData(B, turn);
	PrintGrid(ROW, COL, MAX - 2, S, rdim, cdim);
	DisplayBoard(B, rdim, turn);
	do
	{
		SaveGame(save, B);
		if (StaleMate(B, turn) == true)
		{
			stalemate = true;
			break;
		}
		time_t st = time(0);
		do
		{
			MakeArrayCopy(B, Temp);
			PlayerMsg(turn);
			if (UndoHappen == true)
				DisplayBoard(B, rdim, turn);
			UndoHappen = false;
			do
			{
				PieceToMove(sr, sc, COL, rdim, cdim);
				gotorowcol(12, 70);
				cout << " PIECE SELECTED :" << B[sr][sc];
			} while (IsValidSelection(B, turn, sr, sc) == false);
			HighLight(HB, sr, sc, B, turn);
			PrintGrid(ROW, COL, MAX - 2, S, rdim, cdim);
			DisplayBoard(HB, rdim, turn);
			do
			{
				AskForDestination(er, ec, COL, rdim, cdim);
			} while (IsValidDestination(B, turn, er, ec) == false);
			Castling(B, turn, RWR, LWR, RBR, LBR, WK, BK, sr, sc, er, ec, CastlingHappen);
			if (SelfCheck(B, sr, sc, er, ec, turn) == true)
			{
				Color(4);
				gotorowcol(35, 70);
				cout << "Self Check !";
				Color(15);
			}
			else if (CastlingHappen == true)
				break;
			else if (IsValidMove(B, sr, sc, er, ec, turn) == false)
			{
				Color(4);
				gotorowcol(22, 70);
				cout << "NOT VALID MOVE !";
				Color(15);
			}
			else
			{
				MakeArrayCopy(B, HB);
				Move(HB, sr, sc, er, ec);
				DisplayBoard(HB, rdim, turn);
				Undo(B, Temp, UndoHappen);
			}
		} while (IsValidMove(B, sr, sc, er, ec, turn) == false || SelfCheck(B, sr, sc, er, ec, turn) == true || UndoHappen == true);
		time_t end = time(0);
		time_t timer = end - st;
		if (timer >= 60)
		{
			gotorowcol(20, 70);
			cout << "YOU TOOK " << timer << " SECONDS";
			TimerWin = true;
			break;
		}
		ClearMsg();
		PawnPromotion(B, turn, sr, sc, er, ec, IsPawnPromoted);
		if (CastlingHappen == false)
		{
			if (IsPawnPromoted == false)
				Move(B, sr, sc, er, ec);
		}
		DisplayBoard(B, rdim, turn);
		if (CheckMate(B, turn) == true)
		{
			Color(4);
			gotorowcol(24, 70);
			cout << "CHECK MATE !";
			checkmate = true;
			break;
		}
		if (Check(B, turn) == true)
			DisplayCheck(turn);
		CastlingHappen = false;
		IsPawnPromoted = false;
		TurnChange(turn);
		SaveAndExit(B, turn);
	} while (checkmate == false && stalemate == false && TimerWin == false);
	DisplayWinOrDraw(checkmate, stalemate, TimerWin, turn);
	return _getch();
}




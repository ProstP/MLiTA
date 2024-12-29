/*4.3. Бильярд 2 (6)
Бильярдный стол расчерчен на квадратные клетки и имеет размеры M × N клеток  (2 ≤ M, N ≤ 500). 
В углах стола находятся четыре лузы для шаров. В каждой клетке (i, j), i = 1, 2,..., M; j = 1, 2,..., N задано целое число Cij  ( -1000 ≤  Cij ≤ 1000). 
Шар ставится в центр одной из клеток и после удара может катиться в одном из четырех направлений вдоль диагоналей клетки. Если он достигает края стола, 
то отражается и продолжает движение, а если попадает в угол стола, то сваливается в лузу. Количество отражений не ограничено. 
Требуется выбрать начальную клетку для шара и направление удара так, чтобы
шар попал в одну из луз;
не пересек какую-либо клетку дважды по одной или разным диагоналям;
сумма чисел в клетках, которые шар пересек по диагонали, включая начальную клетку, была максимальной.
Если имеется несколько решений, достаточно дать любое из них. Пример движения шара показан на рисунке. 

Шар начинает движение с правой нижней клетки, проходит в результате все клетки и попадает в верхнюю правую лузу. 
Ввод из файла INPUT.TXT. Первая строка содержит числа M и N через пробел. Каждая i-я строка из следующих M строк содержит N чисел Cij (1 ≤ j ≤ N).
Вывод в файл OUTPUT.TXT. В первой строке выводится максимальная сумма чисел. Во второй строке выводятся через пробел два числа: номера строки и столбца начальной клетки. Нумерация строк и столбцов начинается с 1. В третьей строке двумя заглавными латинскими буквами указывается направление удара: 
DR – вниз и направо;
DL – вниз и налево;
UR – вверх и направо;
UL – вверх и налево.
Считается, что направление “вниз” соответствует увеличению номера строки, а “направо” - увеличению номера столбца.
Пример 
Ввод
2 3
5 -3 7
6 4 -8 
Вывод
22
1 1
DL

*/

#include <iostream>
#include <vector>
#include <ctime>

using Table = std::vector<std::vector<int>>;

const int DR = 0;
const int DL = 1;
const int UR = 2;
const int UL = 3;

struct BallData
{
	int column;
	int row;
	int direction;
};

struct OutputValues
{
	long long sum;
	BallData ball;
};

Table ReadTableValues(int m, int n)
{
	Table area;

	for (int i = 0; i < m; i++)
	{
		std::vector<int> row;
		for (int j = 0; j < n; j++)
		{
			int value;
			std::cin >> value;
			row.push_back(value);
		}

		area.push_back(row);
	}

	return area;
}

BallData CalculateNewBallData(int direction, int oldColumn, int oldRow, int maxColumn, int maxRow)
{
	switch (direction)
	{
	case DR:
		if (oldColumn == maxColumn - 1)
		{
			if (oldRow == maxRow - 1)
			{
				return BallData{ -1, -1, DR };
			}
			else
			{
				return BallData{ oldColumn, oldRow + 1, DL };
			}
		}
		else
		{
			if (oldRow == maxRow - 1)
			{
				return BallData{ oldColumn + 1, oldRow, UR };
			}
			else
			{
				return BallData{ oldColumn + 1, oldRow + 1, DR };
			}
		}
	case DL:
		if (oldColumn == 0)
		{
			if (oldRow == maxRow - 1)
			{
				return BallData{ -1, -1, DL };
			}
			else
			{
				return BallData{ oldColumn, oldRow + 1, DR };
			}
		}
		else
		{
			if (oldRow == maxRow - 1)
			{
				return BallData{ oldColumn - 1, oldRow, UL };
			}
			else
			{
				return BallData{ oldColumn - 1, oldRow + 1, DL };
			}
		}
	case UR:
		if (oldColumn == maxColumn - 1)
		{
			if (oldRow == 0)
			{
				return BallData{ -1, -1, UR };
			}
			else
			{
				return BallData{ oldColumn, oldRow - 1, UL };
			}
		}
		else
		{
			if (oldRow == 0)
			{
				return BallData{ oldColumn + 1, oldRow, DR };
			}
			else
			{
				return BallData{ oldColumn + 1, oldRow - 1, UR };
			}
		}
	case UL:
		if (oldColumn == 0)
		{
			if (oldRow == 0)
			{
				return BallData{ -1, -1, UL };
			}
			else
			{
				return BallData{ oldColumn, oldRow - 1, UR };
			}
		}
		else
		{
			if (oldRow == 0)
			{
				return BallData{ oldColumn - 1, oldRow, DL };
			}
			else
			{
				return BallData{ oldColumn - 1, oldRow - 1, UL };
			}
		}
	}
}

int ReverseDir(int dir)
{
	switch (dir)
	{
	case DR:
		return UL;
	case DL:
		return UR;
	case UL:
		return DR;
	case UR:
		return DL;
	}
}

Table CreateTableWithVisitedCells(int maxRow, int maxColumn)
{
	Table visited;

	for (int i = 0; i < maxRow; i++)
	{
		std::vector<int> row;
		for (int j = 0; j < maxColumn; j++)
		{
			row.push_back(0);
		}
		visited.push_back(row);
	}

	return visited;
}

OutputValues FindSumOfBallPath(const Table& table, int direction, int startRow, int startColumn, int maxRow, int maxColumn)
{
	OutputValues values;
	long long sum = 0;
	values.sum = 0;
	BallData ball{ startColumn, startRow, direction };
	Table visited = CreateTableWithVisitedCells(maxRow, maxColumn);

	while (ball.column != -1 && ball.row != -1 && visited[ball.row][ball.column] != 1)
	{
		sum = sum + table[ball.row][ball.column];
		visited[ball.row][ball.column] = 1;
		if (values.sum < sum)
		{
			values.ball = ball;
			values.sum = sum;
		}
		ball = CalculateNewBallData(ball.direction, ball.column, ball.row, maxColumn, maxRow);
	}

	values.ball.direction = ReverseDir(values.ball.direction);
	return values;
}

std::string GetDirectionInString(int dir)
{
	switch (dir)
	{
	case DR:
		return "DR";
	case DL:
		return "DL";
	case UR:
		return "UR";
	case UL:
		return "UL";
	default:
		return "Unknown direction";
	}
}

OutputValues FindStartPositionWithMaxSum(const Table& table, int maxRow, int maxColumn)
{
	OutputValues maxVals;
	OutputValues ul = FindSumOfBallPath(table, DR, 0, 0, maxRow, maxColumn);
	maxVals = ul;
	OutputValues ur = FindSumOfBallPath(table, DL, 0, maxColumn - 1, maxRow, maxColumn);
	if (maxVals.sum < ur.sum)
	{
		maxVals = ur;
	}
	OutputValues dl = FindSumOfBallPath(table, UR, maxRow - 1, 0, maxRow, maxColumn);
	if (maxVals.sum < dl.sum)
	{
		maxVals = dl;
	}
	OutputValues dr = FindSumOfBallPath(table, UL, maxRow - 1, maxColumn - 1, maxRow, maxColumn);
	if (maxVals.sum < dr.sum)
	{
		maxVals = dr;
	}

	return maxVals;
}

int main()
{
	int m, n;
	std::cin >> m >> n;

	Table table = ReadTableValues(m, n);

	clock_t start = clock();

	OutputValues data = FindStartPositionWithMaxSum(table, m, n);
	std::cout << data.sum << "\n" << data.ball.row + 1 << " " << data.ball.column + 1 << "\n" << GetDirectionInString(data.ball.direction) << "\n";
	std::cout << (clock() - start) / (double)CLOCKS_PER_SEC << "\n";
}

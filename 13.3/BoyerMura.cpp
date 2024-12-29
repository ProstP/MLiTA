/*13.3. Алгоритм Бойера – Мура (6)
Требуется найти все вхождения образца в текстовом файле методом Бойера – Мура. В файле нет
переноса слов. Образец может включать пробелы и переходить с одной строки файла на другую.
Конец строки файла может интерпретироваться как пробел. Результаты поиска не должны
зависеть от регистра букв, то есть каждая буква в образце и файле может быть как строчной, так и
прописной.

Ввод из файла INPUT.TXT. Первая строка файла является образцом и имеет длину от 1 до 255.
Вторая строка задает имя текстового файла.

Вывод в файл OUTPUT.TXT. Вывести в каждой строке через пробел последовательность
номеров строк и позиций в строке, начиная с которых образец входит в текст. Нумерация строк и
позиций в строке начинается с 1. Если вхождений нет, вывести No.

Пример
Ввод
Мама мыла раму
Bukvar.txt

Вывод
1 1
1 23

Пояснение. Файл Bukvar.txt состоит из следующих двух строк:
Мама мыла раму вчера, мама
мыла раму сегодня.

Microsoft Visual Studio 2019, Кузнецов Павел, ПС-23
*/

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <Windows.h>
#include <queue>
#include <ctime>

char ChToLower(char ch)
{
	if (ch == '\n')
	{
		return ' ';
	}
	if ((int)ch <= -33 && (int)ch >= -64)
	{
		return (char)(ch + 32);
	}
	else
	{
		return std::tolower(ch);
	}
	return ch;
}

std::map<char, int> CreateShiftsTable(const std::string& substr)
{
	std::map<char, int> table;

	for (int i = 1; i < substr.length(); i++)
	{
		char symbol = ChToLower(substr[substr.length() - i - 1]);
		if (table.find(symbol) == table.end())
		{
			table[symbol] = i;
		}
	}

	return table;
}

int CompareStrsAndCalculateShiftValue(const std::string& search, const std::string& str, bool& isFound, const std::map<char, int>& table)
{
	int shift = 1;
	char last = ChToLower(str[str.length() - 1]);
	if (table.find(last) != table.end())
	{
		shift = table.at(last);
	}
	else
	{
		shift = search.length();
	}
	isFound = true;
	for (int i = search.length() - 1; i >= 0; i--)
	{
		char chStr = ChToLower(str[i]);
		if (ChToLower(search[i]) != chStr)
		{
			isFound = false;
			break;
		}
	}

	return shift;
}

int CheckStrAndFindShift(std::queue<char> str, std::string substr, bool& wasFound, std::map<char, int> table)
{
	int shift = 1;
	char last = ChToLower(str.back());
	if (table.contains(last))
	{
		shift = table.at(last);
	}
	else
	{
		shift = substr.length();
	}

	wasFound = true;
	for (int i = 0; i < substr.length(); i++)
	{
		if (ChToLower(substr[i]) != ChToLower(str.front()))
		{
			wasFound = false;
			break;
		}
		str.pop();
	}

	return shift;
}

bool PrintSubStrInFile(std::string substr, std::map<char, int> table, std::string fileName)
{
	bool wasFound = false;
	std::ifstream file(fileName);
	int line = 1;
	int pos = 0;
	std::queue<char> queue;
	char ch;
	int shift = substr.length();
	while (file.get(ch))
	{
		shift--;
		queue.push(ch);
		
		if (queue.size() < substr.length())
		{
			continue;
		}
		pos++;
		if (shift != 0)
		{
			if (queue.front() == '\n')
			{
				line++;
				pos = 0;
			}
			queue.pop();
			continue;
		}
		bool isFind;
		shift = CheckStrAndFindShift(queue, substr, isFind, table);
		if (isFind)
		{
			wasFound = true;
			std::cout << line << " " << pos << "\n";
		}
		if (queue.front() == '\n')
		{
			line++;
			pos = 0;
		}
		queue.pop();
	}

	return wasFound;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	clock_t start = clock();
	std::string substr;
	std::string fileName;
	std::getline(std::cin, substr);
	std::getline(std::cin, fileName);
	auto shiftsTable = CreateShiftsTable(substr);

	if (!PrintSubStrInFile(substr, shiftsTable, fileName))
	{
		std::cout << "No";
	}

	std::cout << (float)(clock() - start) / CLOCKS_PER_SEC << "\n";
}

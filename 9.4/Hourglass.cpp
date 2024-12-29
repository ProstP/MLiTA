/*9.4. Песочные часы (6)
Имеется коллекция из N песочных часов. Часы с номером i имеют две характеристики: Ai – максимальная длительность их работы в секундах, 
Bi – время в секундах, которое уже прошло с запуска часов. 
Требуется найти минимальное количество переворотов часов, чтобы все часы закончили свою работу друг за другом с разницей в 1 сек.

Ввод. 
Первая строка входных данных содержит единственное целое число N (1 ≤ N ≤ 20) – количество часов.  
Вторая строка входных данных содержит N целых чисел A1, A2, ..., AN (0 ≤ Ai ≤ 104) – длительность работы часов в секундах. 
Третья строка входных данных содержит N целых чисел B1, B2, ..., BN (0 ≤ Bi ≤ Ai ≤ 104) - время, которое прошло с запуска часов в секундах.

Вывод. 
Выведите единственное целое число - минимальное количество переворотов часов, требующихся для выполнения условия. 
Если условие выполнить невозможно, то выведите -1.
Примеры
Ввод 1         Ввод 2           Ввод 3
3              5                4
5 4 9          1 3 5 7 9        13 13 16 14
3 3 6          1 3 5 7 9        9 5 6 7
Вывод 1        Вывод 2          Вывод 3
0              -1               1
*/
// Кузнецов Павел ПС-23, Microsoft Visual Studio 2019

#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <set>

struct Combination
{
	std::set<int> digits;
	int flitsCount = 0;
};

std::vector<int> ReadValues(int num)
{
	std::vector<int> values;

	for (int i = 0; i < num; i++)
	{
		int value;
		std::cin >> value;
		values.push_back(value);
	}

	return values;
}

bool IsDifferentToOne(std::set<int> set)
{
	int minValue = *set.begin();

	for (int i = minValue; i < minValue + set.size(); i++)
	{
		if (!set.contains(i))
		{
			return false;
		}
	}

	return true;
}

bool IsNewValueFarFromSetValues(int value, const std::set<int>& set, int difference)
{
	if (value < *set.begin() - difference || value > *set.begin() + difference)
	{
		return true;
	}

	return false;
}

void InsertNewCombination(int value, const Combination& oldSet, std::vector<Combination>& vector, bool isFlit)
{
	Combination newSet = oldSet;
	newSet.digits.insert(value);
	if (isFlit)
	{
		newSet.flitsCount++;
	}
	vector.push_back(newSet);
}

std::vector<Combination> InsertNewTimesToSets(int spended, int remained, const std::vector<Combination>& oldTimes, int differnece)
{
	std::vector<Combination> newTimes;

	for (const auto& set : oldTimes)
	{
		if (!set.digits.contains(remained) && !IsNewValueFarFromSetValues(remained, set.digits, differnece))
		{
			InsertNewCombination(remained, set, newTimes, false);
		}
		if (spended != remained && !set.digits.contains(spended) && !IsNewValueFarFromSetValues(spended, set.digits, differnece))
		{
			InsertNewCombination(spended, set, newTimes, true);
		}
	}

	return newTimes;
}

int FindMinCountOfFlits(int num, const std::vector<int>& clockTimes, const std::vector<int>& spendedTime)
{
	std::vector<Combination> timesSets;

	timesSets.push_back(Combination{ std::set<int>{clockTimes[0] - spendedTime[0]}, 0 });
	timesSets.push_back(Combination{ std::set<int>{spendedTime[0]}, 1 });

	for (int i = 1; i < num; i++)
	{
		timesSets = InsertNewTimesToSets(spendedTime[i], clockTimes[i] - spendedTime[i], timesSets, num);
	}

	int minFlits = INT_MAX;
	for (auto& set : timesSets)
	{
		if (minFlits <= set.flitsCount)
		{
			continue;
		}
		if (IsDifferentToOne(set.digits))
		{
			minFlits = set.flitsCount;
		}
	}

	if (minFlits == INT_MAX)
	{
		minFlits = -1;
	}

	return minFlits;
}

int main()
{
	int num;
	std::cin >> num;

	std::vector<int> clockTimes = ReadValues(num);
	std::vector<int> spendedTimes = ReadValues(num);

	clock_t start = clock();

	std::cout << FindMinCountOfFlits(num, clockTimes, spendedTimes) << "\n\n";

	std::cout << "Time: " << (clock() - start) / (float)CLOCKS_PER_SEC << "\n";
}
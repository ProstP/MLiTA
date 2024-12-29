/*14.10. Распил бруса 2 (10)
На пилораму привезли брус длиной L метров. Требуется сделать N распилов. Распилы делят брус на части, 
длина которых выражается натуральными числами. Стоимость одного распила равна длине распиливаемого бруса. 
Определить минимальную стоимость распила.

Ввод. В первой строке содержатся через пробел натуральные числа L (2 ≤ L ≤ 10^5) и N (N < L) – длина бруса и число распилов.
Вывод. В единственной строке вывести минимальную стоимость распилов.

Кузнецов Павел ПС-23, Microsoft Visual Studio 2019
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>

void InsertValueToVector(std::vector<int>& timberParts, int value, int count)
{
	if (timberParts.size() == 0)
	{
		timberParts.insert(timberParts.cend(), count, value);
		return;
	}

	if (value < timberParts.back())
	{
		timberParts.insert(timberParts.cend() - 1, count, value);
	}
	else
	{
		timberParts.insert(timberParts.cend(), count, value);
	}
}

int CalculateMinCost(int lengthOfTimber, int numOfCost)
{
	std::vector<int> timberParts;
	int minCost = 0;
	int numOfPairs = 0;
	int currentSum = 1;

	if (lengthOfTimber - numOfCost != 1)
	{
		InsertValueToVector(timberParts, lengthOfTimber - numOfCost, 1);
		numOfPairs = numOfCost;
	}
	else
	{
		numOfPairs = numOfCost + 1;
	}

	while (numOfPairs != 1)
	{
		if (numOfPairs % 2 == 1)
		{
			InsertValueToVector(timberParts, currentSum, 1);
			numOfPairs--;
			numOfPairs /= 2;
			currentSum *= 2;
			minCost = minCost + currentSum * numOfPairs;
			break;
		}

		numOfPairs /= 2;
		currentSum *= 2;
		minCost = minCost + currentSum * numOfPairs;
	}

	InsertValueToVector(timberParts, currentSum, numOfPairs);

	while (timberParts.size() > 1)
	{
		int firstPart = timberParts[0];
		int secondPart = timberParts[1];

		timberParts.erase(timberParts.cbegin(), timberParts.cbegin() + 2);
		int sum = firstPart + secondPart;
		int num = 1;
		while (timberParts.size() > 1 && timberParts[0] == firstPart && timberParts[1] == firstPart)
		{
			timberParts.erase(timberParts.cbegin(), timberParts.cbegin() + 2);
			num++;
		}
		minCost = minCost + sum * num;
		InsertValueToVector(timberParts, sum, num);
	}

	return minCost;
}

int main()
{
	int lengthOfTimber, numOfCuts;
	std::cin >> lengthOfTimber >> numOfCuts;
	clock_t start = clock();

	int sum = CalculateMinCost(lengthOfTimber, numOfCuts);
	std::cout << sum << "\n";
	std::cout << (clock() - start) / (float)CLOCKS_PER_SEC << "\n";
}

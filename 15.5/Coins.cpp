/*15.5. Монеты (7)
Однокопеечные монетки разложены в N стопок (1 <= N <= 100). В стопках может быть различное
количество монет, а стопки поставлены на столе в ряд слева направо. Двое противников по
очереди делают ходы. Ход состоит в том, что один из игроков берет слева несколько стопок
подряд, не меньше одной, но и не больше, чем перед этим взял его соперник. Первый игрок своим
первым ходом берет не более K стопок (1 <= K <= 80). Игра заканчивается, когда стопок не остается.
Требуется найти максимальное число монет, которое может получить первый участник после
окончания игры, если второй игрок тоже старается ходить так, чтобы получить как можно больше
монет.

Ввод из файла INPUT.TXT. В первой строке находятся числа стопок N и K через пробел. Во
второй строке идут через пробел N чисел, задающих количество монет в стопках слева направо.
Количество монет в каждой стопке не превышает 1000.

Вывод в файл OUTPUT.TXT. Вывести одно число - максимальное количество монет, которое
заведомо может получить первый игрок, как бы ни играл второй.

Примеры
    Ввод 1      Ввод 2      Ввод 3
    3 3         4 3         5 2
    4 9 1       1 2 2 7     3 4 8 1 7

    Вывод 1     Вывод 2     Вывод 3
    14          5           18
*/

#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

struct Step
{
    int position;
    int lastStep;
};

std::vector<int> ReadNums(int n)
{
    std::vector<int> nums;

    for (int i = 0; i < n; i++)
    {
        int value;
        std::cin >> value;
        nums.push_back(value);
    }

    return nums;
}

std::vector<int> CalculateSumsRemainElts(const std::vector<int>& coins)
{
    std::vector<int> r;
    for (int i = 0; i < coins.size(); i++)
    {
        int sum = 0;

        for (int j = i; j < coins.size(); j++)
        {
            sum += coins[j];
        }
        r.push_back(sum);
    }

    return r;
}

void CalculateCoinsForStep(int pos, int last, std::vector<std::vector<int>>& steps, int n, const std::vector<int>& r)
{
    if (pos + last > n)
    {
        steps[pos][last] = r[pos - 1];
        return;
    }

    std::vector<int> nextSteps;
    
    for (int i = 1; i <= last; i++)
    {
        if (steps[pos + i][i] == 0)
        {
            CalculateCoinsForStep(pos + i, i, steps, n, r);
        }

        nextSteps.push_back(steps[pos + i][i]);
    }
    int min = *std::min_element(nextSteps.begin(), nextSteps.end());
    steps[pos][last] = r[pos - 1] - min;
}

void CalculateCoins(std::vector<std::vector<int>>& steps, int n, int k, const std::vector<int>& r)
{
    CalculateCoinsForStep(1, k, steps, n, r);
}

int main()
{
    int n, k;
    std::cin >> n >> k;

    auto coins = ReadNums(n);
    
    clock_t start = clock();
    std::vector<std::vector<int>> steps(n + 1, std::vector<int>(k + 1, 0));
    auto r = CalculateSumsRemainElts(coins);
    CalculateCoins(steps, n, k, r);
    std::cout << steps[1][k] << "\n";
    std::cout << (clock() - start) / (float)CLOCKS_PER_SEC;
}

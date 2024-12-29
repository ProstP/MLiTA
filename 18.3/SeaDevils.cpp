/*18.3. Морские дьяволы (8)
Решить задачу 12.9 при ограничениях (1 <= M, N <= 1500). Использовать при решении бинарную
кучу.

12.9. Морские дьяволы
Полигон для тренировки морских десантников представляет собой площадку в форме
прямоугольника с водоемами и задается матрицей размером M x N. Каждый элемент матрицы
содержит либо символ '@', обозначающий участок водной поверхности, либо символ '.' (точка),
обозначающий участок суши. Подразделение морских дьяволов находится в клетке,
соответствующей левому верхнему углу матрицы. Ему поставлена задача достичь участка,
соответствующего правому нижнему углу матрицы. Десантники могут передвигаться в
направлениях вдоль сторон полигона, не выходя за его пределы. Они планируют преодолеть как
можно меньше клеток, занятых водой. Если это можно сделать по-разному, то предпочтительнее
такой вариант, когда путь включает меньшее количество клеток суши.

Ввод из файла INPUT.TXT. В первой строке содержатся числа M и N (1 <= M, N <= 300),
разделенные пробелами. В следующих M строках находится матрица, представляющая полигон,
по N подряд идущих символов в строке. Гарантируется, что левый верхний и правый нижний углы
матрицы соответствуют участкам суши.

Вывод в файл OUTPUT.TXT. В единственной строке вывести через пробел наименьшее число
клеток K, которое подразделение должно преодолеть по воде, и для найденного значения K
минимальное число клеток по суше L, включая начальную и конечную клетки.

Примеры
    Ввод 1      Ввод 2
    7 7         7 6
    ..@@...     .@@@..
    ..@@@..     ......
    @.@@..@     @.@@.@
    @@@...@     @@@..@
    ..@....     ..@...
    ..@...@     ..@...
    ....@..     ....@.

    Вывод 1     Вывод 2
    1 14        0 12

Кузнцеов Павел, ПС-23, Microsoft Visual Studio 2019
*/
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

struct Cell
{
    //int inBinaryHeap = -1;
    bool inHeap = false;
    bool isWater = false;
    bool isVisited = false;
};

struct Way
{
    unsigned short x = 0;
    unsigned short y = 0;
    int waterCount = 0;
    int landCount = 0;
};

std::vector<std::vector<Cell>> ReadArea(unsigned short m, unsigned short n)
{
    std::vector<std::vector<Cell>> area(m, std::vector<Cell>(n, Cell{}));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            char ch;
            std::cin >> ch;
            if (ch == '@')
            {
                area[i][j].isWater = true;
            }
        }
    }

    return area;
}

void SiftBinaryHeap(int index, std::vector<Way>& binaryHeap, std::vector<std::vector<Cell>>& area)
{
    while (index != 0)
    {
        int parentIndex = (index + 1) / 2 - 1;
        if (binaryHeap[index].waterCount < binaryHeap[parentIndex].waterCount ||
            (binaryHeap[index].waterCount == binaryHeap[parentIndex].waterCount && binaryHeap[index].landCount < binaryHeap[parentIndex].landCount))
        {
            Way temp = binaryHeap[index];
            binaryHeap[index] = binaryHeap[parentIndex];
            binaryHeap[parentIndex] = temp;
            index = parentIndex;
        }
        else
        {
            break;
        }
    }
}

void InsertNewWayToBinaryHeap(const Way& way, std::vector<Way>& binaryHeap, std::vector<std::vector<Cell>>& area)
{
    binaryHeap.push_back(way);
    int index = binaryHeap.size() - 1;
    SiftBinaryHeap(index, binaryHeap, area);
}

int FindInBinaryHeap(int x, int y, std::vector<Way>& binaryHeap)
{
    for (int i = 0; i < binaryHeap.size(); i++)
    {
        if (binaryHeap[i].x == x && binaryHeap[i].y == y)
        {
            return i;
        }
    }

    return 0;
}

void ChangeWayInBinaryHeap(unsigned short x, unsigned short y, int waterCount, int landCount,
    std::vector<Way>& binaryHeap, std::vector<std::vector<Cell>>& area)
{
    if (area[x][y].isVisited)
    {
        return;
    }
    int isWater = static_cast<int>(area[x][y].isWater);
    if (!area[x][y].inHeap)
    {
        area[x][y].inHeap = true;
        InsertNewWayToBinaryHeap(Way{x, y, waterCount + isWater, landCount + 1}, binaryHeap, area);
    }
    else
    {
        int index = FindInBinaryHeap(x, y, binaryHeap);
        if (binaryHeap[index].waterCount < waterCount + isWater ||
            (binaryHeap[index].waterCount == waterCount + isWater
                && binaryHeap[index].landCount < landCount + 1))
        {
            return;
        }
        binaryHeap[index].landCount = landCount + 1;
        binaryHeap[index].waterCount = waterCount + isWater;
        SiftBinaryHeap(index, binaryHeap, area);
    }
}

Way GetTopOfHeap(std::vector<Way>& binaryHeap)
{
    Way top = binaryHeap[0];
    //binaryHeap[0] = binaryHeap[binaryHeap.size() - 1];
    //binaryHeap.erase(binaryHeap.end() - 1);
    int index = 0;
    while ((index + 1) * 2 - 1 < binaryHeap.size())
    {
        int child1Index = (index + 1) * 2 - 1;
        int child2Index = (index + 1) * 2;
        int minIndex = 0;
        if (child2Index == binaryHeap.size())
        {
            minIndex = child1Index;
        }
        else
        {
            int water1Min = binaryHeap[child1Index].waterCount;
            int land1Min = binaryHeap[child1Index].landCount;
            int water2Min = binaryHeap[child2Index].waterCount;
            int land2Min = binaryHeap[child2Index].landCount;
            minIndex = water1Min < water2Min ? child1Index : (water1Min == water2Min ? (land1Min < land2Min ? child1Index : child2Index) : child2Index);
        }
        if (binaryHeap[index].waterCount > binaryHeap[minIndex].waterCount ||
            (binaryHeap[index].waterCount == binaryHeap[minIndex].waterCount && binaryHeap[index].landCount > binaryHeap[minIndex].landCount))
        {
            Way temp = binaryHeap[index];
            binaryHeap[index] = binaryHeap[minIndex];
            binaryHeap[minIndex] = temp;
        }
        index = minIndex;
    }

    return top;
}

Way FindMinWay(std::vector<std::vector<Cell>>& area, unsigned short m, unsigned short n)
{
    std::vector<Way> binaryHeap(1, Way{0, 0, 0, 1});
    Way cur;

    while (!binaryHeap.empty())
    {
        cur = GetTopOfHeap(binaryHeap);
        if (cur.x == m - 1 && cur.y == n - 1)
        {
            return cur;
        }
        area[cur.x][cur.y].isVisited = true;
        if (cur.x < m - 1 && !area[cur.x + 1][cur.y].isVisited)
        {
            ChangeWayInBinaryHeap(cur.x + 1, cur.y, cur.waterCount, cur.landCount, binaryHeap, area);
        }
        if (cur.y < n - 1 && !area[cur.x][cur.y + 1].isVisited)
        {
            ChangeWayInBinaryHeap(cur.x, cur.y + 1, cur.waterCount, cur.landCount, binaryHeap, area);
        }
        if (cur.x > 0 && !area[cur.x - 1][cur.y].isVisited)
        {
            ChangeWayInBinaryHeap(cur.x - 1, cur.y, cur.waterCount, cur.landCount, binaryHeap, area);
        }
        if (cur.y > 0 && !area[cur.x][cur.y - 1].isVisited)
        {
            ChangeWayInBinaryHeap(cur.x, cur.y - 1, cur.waterCount, cur.landCount, binaryHeap, area);
        }
    }

    return cur;
}

int main()
{
    unsigned short m, n;
    std::cin >> m >> n;
    auto area = ReadArea(m, n);
    clock_t start = clock();
    auto way = FindMinWay(area, m, n);
    std::cout << way.waterCount << " " << way.landCount -way.waterCount << "\n";
    std::cout << (float)(clock() - start) / CLOCKS_PER_SEC;
}

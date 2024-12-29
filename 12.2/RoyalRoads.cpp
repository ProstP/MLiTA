/*12.2. Королевские дороги (9)
В некотором королевстве N городов соединены N-1 дорогой. Имеется ровно один путь между
любой парой городов. Дороги иногда выходят из строя. Требуется построить минимальное
количество новых дорог так, чтобы каждая пара городов оказалась бы связана даже в случае
выхода из строя какой-либо дороги.

Ввод. В первой строке N (2 ≤ N ≤ 100000). В следующих N-1 строках – дороги в виде пар
номеров городов A i , B i (1 ≤ A i , B i ≤ N).

Вывод. В первой строке минимальное число K новых дорог. В следующих K строках – пары
городов, задающих новые дороги.

Примеры
    Ввод 1      Ввод 2
    5           4
    1 2         1 2
    2 3         1 3
    3 4         1 4
    3 5
    Вывод 1     Вывод 2
    2           2
    1 4         3 2
    4 5         1 4

Microsoft Visaul Studio 2019, Кузнецов Павел, ПС-23
*/
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <set>
#include <queue>

struct Town
{
    std::vector<int> children;
    std::vector<int> newChildren;
};

struct Road
{
    int start;
    int end;
};

std::vector<Town> ReadRoads(int n)
{
    std::vector<Town> towns(n + 1, Town());

    int start, end;

    std::cin >> start >> end;
    towns[start].children.push_back(end);
    towns[end].children.push_back(start);
    towns[0].children.push_back(start);

    for (int i = 1; i < n - 1; i++)
    {
        std::cin >> start >> end;
        towns[start].children.push_back(end);
        towns[end].children.push_back(start);
    }

    return towns;
}

int FindEndOfChild(int index, std::vector<Town>& towns, std::set<int> visited)
{
    int current = index;
    while (towns[current].children.size() != 1)
    {
        visited.insert(current);
        for (int i = 0; i < towns[current].children.size(); i++)
        {
            int child = towns[current].children[i];
            if (!visited.contains(child) && towns[child].newChildren.empty())
            {
                current = child;
                break;
            }
            if (i == towns[current].children.size() - 1)
            {
                return 0;
            }
        }
    }
    return current;
}

void InsertTownToConnect(int index, std::queue<int>& toConnect, std::vector<Town>& towns, std::vector<Road>& roads, int& lastConnect)
{
    if (toConnect.empty())
    {
        toConnect.push(index);
        return;
    }

    int townToCon = toConnect.front();
    if (townToCon == index)
    {
        return;
    }
    toConnect.pop();
    
    towns[index].newChildren.push_back(townToCon);
    towns[townToCon].newChildren.push_back(index);
    lastConnect = index;
    roads.push_back(Road{ townToCon, index });

    return;
}

void FindChildrenToConnect(int index, std::vector<Town>& towns, std::vector<Road>& roads, 
                            std::queue<int>& toConnect, const std::set<int>& visited, int& lastConnect)
{
    auto town = towns[index];

    for (int i : town.children)
    {
        if (!towns[i].newChildren.empty() || visited.contains(i))
        {
            continue;
        }

        int endChild = FindEndOfChild(i, towns, visited);
        if (endChild == 0)
        {
            continue;
        }
        InsertTownToConnect(endChild, toConnect, towns, roads, lastConnect);
    }
}

std::vector<Road> ConnectTowns(int root, std::vector<Town>& towns)
{
    std::vector<Road> roads;
    std::set<int> visitingTowns;
    std::queue<int> townsToVisit;
    std::queue<int> townsToConnect;
    bool rootNeed = false;
    int lastConnect;
    
    if (towns[root].children.size() == 1)
    {
        rootNeed = true;
        townsToVisit.push(towns[root].children[0]);
        visitingTowns.insert(root);
    }
    else
    {
        townsToVisit.push(root);
    }

    while (!townsToVisit.empty())
    {
        int curr = townsToVisit.front();
        visitingTowns.insert(curr);
        townsToVisit.pop();
        int c = 0;

        for (int i : towns[curr].children)
        {
            if (!visitingTowns.contains(i) && !towns[i].children.empty())
            {
                townsToVisit.push(i);
                c++;
            }
        }

        if (c != 1)
        {
            FindChildrenToConnect(curr, towns, roads, townsToConnect, visitingTowns, lastConnect);
        }
    }

    if (!townsToConnect.empty())
    {
        roads.push_back(Road{root, townsToConnect.front()});
    }
    else if (rootNeed)
    {
        roads.push_back(Road{root, lastConnect});
    }

    return roads;
}

int main()
{
    auto start = clock();
    int n;
    std::cin >> n;

    auto towns = ReadRoads(n);
    int root = towns[0].children[0];
    auto roads = ConnectTowns(root, towns);

    std::cout << roads.size() << "\n";

    for (auto road : roads)
    {
        std::cout << road.start << " " << road.end << "\n";
    }

    std::cout << "\nTime: " << (clock() - start) / (float)CLOCKS_PER_SEC << "\n";
}
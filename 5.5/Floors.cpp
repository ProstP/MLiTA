/*5.5. Этажи (7)
Новое здание кампуса университета имеет n этажей, пронумерованных снизу вверх от 1 до n.
Комнаты студентов расположены в нескольких подъездах.
В каждом подъезде на этажах, номер которых кратен числу k, расположено по x комнат, а на
остальных этажах расположено по y комнат.
Комнаты внутри каждого подъезда пронумерованы последовательными натуральными
числами. Номера комнат на первом этаже имеют наименьшие значения в этом подъезде, затем
следуют номера комнат на втором этаже, и так далее. Комнаты в первом подъезде пронумерованы,
начиная с 1, в каждом следующем подъезде нумерация комнат начинается с числа, следующего
после максимального номера комнаты в предыдущем подъезде.
На рис. показаны номера комнат в здании с n = 7 этажами, 3 подъездами, и параметрами k = 3,
x = 2, y = 3.

        Подъезд 1       Подъезд 2       Подъезд 3
7 этаж  17, 18, 19      36, 37, 38      55, 56, 57
6 этаж  15, 16          34, 35          53, 54
5 этаж  12, 13, 14      31, 32, 33      50, 51, 52
4 этаж  9,  10, 11      28, 29, 30      47, 48, 49
3 этаж  7,  8           26, 27          45, 46
2 этаж  4,  5,  6       23, 24, 25      42, 43, 44
1 этаж  1,  2,  3       20, 21, 22      39, 40, 41

Для организации расселения студентов администрация кампуса должна по номеру комнаты
оперативно определять этаж, на котором она находится.
Требуется написать программу, которая по заданным числам n, k, x и y, а также по номерам
комнат, определяет для каждой комнаты, на каком этаже она находится.
Ввод из файла INPUT.TXT. Первая строка входного файла содержит натуральные числа n, k, x
и y (1 ≤ n ≤ 10^9 , 1 ≤ k ≤ n, 1 ≤ x, y ≤ 10^9 ). Соседние числа разделены ровно одним пробелом. Вторая
строка входного файла содержит натуральное число q — количество номеров комнат, для которых
требуется определить этаж (1 ≤ q ≤ 1000). Третья строка содержит q целых чисел a 1 , a 2 , …, a q —
номера комнат (1 ≤ a i  ≤ 10^18 ). Можно считать, что в здании так много подъездов, что все комнаты с
заданными номерами существуют.
Вывод в файл OUTPUT.TXT. Требуется вывести q чисел, по одному на строке. Для
каждого номера комнаты во входном файле требуется вывести номер этажа, на котором
она находится.
Примеры
Ввод
7 3 2 3
4
1 19 20 50
Вывод
1
7
1
5
*/

#include <iostream>
#include <vector>
#include <ctime>

std::vector<long long> ReadRooms(int q)
{
    std::vector<long long> rooms;
    for (int i = 0; i < q; i++)
    {
        long long room;
        std::cin >> room;
        rooms.push_back(room);
    }

    return rooms;
}

long long FindRoomCountInEntrance(long long n, long long k, long long x, long long y)
{
    int multiplesCount = n / k;
    long long roomCount = multiplesCount * x + (n - multiplesCount) * y;
    return roomCount;
}

int FindFloorOfRoom(long long room, long long roomCount, long long n, long long k, long x, long long y)
{
    room %= roomCount;
    if (room == 0)
    {
        return n;
    }

    if (roomCount - room < y * (n % k))
    {
        return n - (roomCount - room) / y;
    }

    int countGroupRoomsToK = (k - 1) * y + x;
    int group = room / countGroupRoomsToK;
    int roomPostionInGroup = room % countGroupRoomsToK;
    int roomFloorInGroup;
    if (countGroupRoomsToK - roomPostionInGroup < x)
    {
        roomFloorInGroup = k;
    }
    else
    {
        roomFloorInGroup = roomPostionInGroup / y;
        if (roomPostionInGroup % y != 0)
        {
            roomFloorInGroup++;
        }
    }

    int floor = k * group + roomFloorInGroup;
    
    return floor;
}

int main()
{
    int n, k, x, y, q;
    std::cin >> n >> k >> x >> y >> q;
    std::vector<long long> rooms = ReadRooms(q);
    clock_t start = clock();
    long long roomCount = FindRoomCountInEntrance(n, k, x, y);
    std::cout << "\n";
    for (const long long& room : rooms)
    {
        int floor = FindFloorOfRoom(room, roomCount, n, k, x, y);
        std::cout << floor << "\n";
    }
    std::cout << "\nTime: " << (clock() - start) / (float)CLOCKS_PER_SEC << "\n";
}
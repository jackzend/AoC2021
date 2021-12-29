#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct hash_pair
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};
int NUM_COL = 0;
int NUM_ROW = 0;

int STEPS = 1;

enum class CucumberType
{
    EMPTY,
    EAST,
    SOUTH
};

char toChar(CucumberType t)
{
    if (t == CucumberType::EMPTY)
    {
        return '.';
    }
    if (t == CucumberType::SOUTH)
    {
        return 'v';
    }
    if (t == CucumberType::EAST)
    {
        return '>';
    }
}

CucumberType toEnum(char c)
{
    if (c == '.')
    {
        return CucumberType::EMPTY;
    }
    if (c == 'v')
    {
        return CucumberType::SOUTH;
    }
    if (c == '>')
    {
        return CucumberType::EAST;
    }
}

unordered_map<pair<int, int>, CucumberType, hash_pair>
    cucumber_map;

void printMap()
{
    for (int i = 0; i < NUM_ROW; ++i)
    {
        for (int j = 0; j < NUM_COL; ++j)
        {
            cout << toChar(cucumber_map[make_pair(i, j)]);
        }
        cout << endl;
    }
    cout << endl;
}

void parseInput()
{
    ifstream f;
    f.open("day25.txt");

    string temp;
    int row = 0;
    while (getline(f, temp))
    {
        NUM_COL = temp.size();

        for (int col = 0; col < temp.size(); ++col)
        {
            cucumber_map[make_pair(row, col)] = toEnum(temp[col]);
        }
        ++row;
    }

    NUM_ROW = row;

    printMap();
}

pair<int, int> getEastCoord(pair<int, int> curr)
{
    pair<int, int> next;
    next.first = curr.first;        // same row
    if (curr.second == NUM_COL - 1) // on the right edge
    {
        next.second = 0; // go back to zero
    }
    else
    {
        next.second = curr.second + 1; // else just increment one to the right
    }
    return next;
}

bool moveEast()
{
    bool someoneMoved = false;
    vector<pair<pair<int, int>, CucumberType>> inserters; // first is the coordinate, secodn is the new value

    for (auto &[coord, cuc_type] : cucumber_map)
    {

        if (cuc_type == CucumberType::EAST)
        {
            auto right_coord = getEastCoord(coord);
            if (cucumber_map[right_coord] == CucumberType::EMPTY)
            {
                someoneMoved = true;
                auto moved_into_pair = make_pair(right_coord, CucumberType::EAST);
                auto moved_from_pair = make_pair(coord, CucumberType::EMPTY);
                inserters.push_back(moved_into_pair);
                inserters.push_back(moved_from_pair);
            }
        }
    }

    for (auto &ins : inserters)
    {
        cucumber_map[ins.first] = ins.second;
    }

    return someoneMoved;
}

pair<int, int> getSouthCoord(pair<int, int> curr)
{
    pair<int, int> next;
    next.second = curr.second;     // same col
    if (curr.first == NUM_ROW - 1) // on the bottom edge
    {
        next.first = 0; // go back to zero
    }
    else
    {
        next.first = curr.first + 1; // else just increment one down
    }
    return next;
}

bool moveSouth()
{
    bool someoneMoved = false;
    vector<pair<pair<int, int>, CucumberType>> inserters; // first is the coordinate, secodn is the new value

    for (auto &[coord, cuc_type] : cucumber_map)
    {

        if (cuc_type == CucumberType::SOUTH)
        {
            auto down_coord = getSouthCoord(coord);
            if (cucumber_map[down_coord] == CucumberType::EMPTY)
            {
                someoneMoved = true;
                auto moved_into_pair = make_pair(down_coord, CucumberType::SOUTH);
                auto moved_from_pair = make_pair(coord, CucumberType::EMPTY);
                inserters.push_back(moved_into_pair);
                inserters.push_back(moved_from_pair);
            }
        }
    }

    for (auto &ins : inserters)
    {
        cucumber_map[ins.first] = ins.second;
    }

    return someoneMoved;
}

void partOne()
{
    for (int i = 0; true; ++i)
    {
        bool first = moveEast();
        bool second = moveSouth();
        //printMap();
        if (first or second) // good
        {
            continue;
        }
        else
        {
            cout << "Stopped on " << i + 1 << endl;
            break;
        }
    }
}

int main()
{
    parseInput();
    partOne();
}
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <utility>
#include <set>

using namespace std;

vector<vector<int>> v;

using rc_pair = pair<int, int>;

void parseInput()
{
    ifstream f;
    f.open("day11.txt");

    string temp;
    while (getline(f, temp))
    {
        vector<int> row;
        row.push_back(-1);
        for (auto &c : temp)
        {
            string t = "";
            t += c;
            row.emplace_back(stoi(t));
        }
        row.push_back(-1);
        v.push_back(row);
    }
    vector<int> buff(v[0].size(), -1);

    v.push_back(buff);
    v.insert(begin(v), buff);
    for (auto &row : v)
    {
        for (auto &i : row)
        {
            cout << i << " ";
        }
        cout << endl;
    }
}

void stepOne()
{
    for (int i = 1; i < v.size() - 1; ++i)
    {
        for (int j = 1; j < v[0].size() - 1; ++j)
        {
            v[i][j] += 1;
        }
    }
}

vector<rc_pair> genNeighbors(rc_pair &p)
{
    vector<rc_pair> ret;
    ret.emplace_back(make_pair(p.first - 1, p.second));
    ret.emplace_back(make_pair(p.first + 1, p.second));
    ret.emplace_back(make_pair(p.first + 1, p.second + 1));
    ret.emplace_back(make_pair(p.first + 1, p.second - 1));
    ret.emplace_back(make_pair(p.first - 1, p.second + 1));
    ret.emplace_back(make_pair(p.first - 1, p.second - 1));
    ret.emplace_back(make_pair(p.first, p.second + 1));
    ret.emplace_back(make_pair(p.first, p.second - 1));

    return ret;
}

bool isValid(rc_pair &p)
{
    if (v[p.first][p.second] == 0)
    {
        return false;
    }
    if (v[p.first][p.second] == 10)
    {
        return false;
    }
    if (v[p.first][p.second] == -1)
    {
        return false;
    } // this is a border
    return true;
}

int sneakySearch()
{

    vector<rc_pair> init;

    for (int i = 1; i < v.size() - 1; ++i)
    {
        for (int j = 1; j < v[0].size() - 1; ++j)
        {
            if (v[i][j] >= 10)
            {

                init.emplace_back(make_pair(i, j));
            }
        }
    }

    int flashed_i = 0;

    for (auto &p : init)
    {

        stack<rc_pair> s;
        set<rc_pair> flashed;

        s.push(p);
        flashed.insert(p);
        v[p.first][p.second] = 0; // switch to zero when we flash
        flashed_i++;

        while (!s.empty())
        {
            auto curr = s.top();
            s.pop();

            auto v_t = genNeighbors(curr);

            for (auto &neighbor : v_t)
            {
                if (isValid(neighbor) and flashed.find(neighbor) == flashed.end())
                {
                    // cout << v[neighbor.first][neighbor.second] << endl;
                    v[neighbor.first][neighbor.second] += 1;
                    if (v[neighbor.first][neighbor.second] >= 10)
                    {
                        v[neighbor.first][neighbor.second] = 0;
                        s.push(neighbor);
                        flashed.insert(neighbor);
                        flashed_i += 1;
                    }
                }
            }
        }
    }
    return flashed_i;
}

void print_v()
{
    for (int i = 0; i < v.size(); ++i)
    {
        for (int j = 0; j < v[0].size(); ++j)
        {
            cout << v[i][j];
        }
        cout << endl;
    }
    cout << endl;
}
void partOne()
{

    long long int sum = 0;
    for (int i = 0; i < 100; ++i)
    {

        stepOne();
        print_v();
        sum += sneakySearch();
        print_v();
    }

    cout << sum << endl;
}

bool isSynched()
{
    bool allFlashed = true;
    for (int i = 1; i < v.size() - 1; ++i)
    {
        for (int j = 1; j < v[0].size() - 1; ++j)
        {
            if (v[i][j] != 0)
            {
                return false;
            }
        }
    }

    return true;
}

void partTwo()
{
    int step = 0;
    while (!isSynched())
    {
        stepOne();
        sneakySearch();
        ++step;
    }
    cout << step;
}

int main()
{
    parseInput();
    //partOne();
    partTwo();
}
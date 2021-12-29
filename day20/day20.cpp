#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
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

unordered_map<pair<int, int>, char, hash_pair> ump;
string BIT_STRING;
void parseInput()
{
    ifstream f;
    f.open("day20.txt");

    getline(f, BIT_STRING);

    cout << BIT_STRING << endl
         << endl;

    string temp;
    getline(f, temp);

    vector<vector<char>> temp_v;
    while (getline(f, temp))
    {
        vector<char> row;
        row.push_back('.');
        row.push_back('.');
        row.push_back('.');
        row.push_back('.');
        row.push_back('.');
        row.push_back('.');
        for (int i = 0; i < temp.size(); ++i)
        {
            row.push_back(temp[i]);
        }
        row.push_back('.');
        row.push_back('.');
        row.push_back('.');
        row.push_back('.');
        row.push_back('.');
        row.push_back('.');
        temp_v.push_back(row);
    }

    // insert padding

    vector<char> padding(temp_v[0].size(), '.');

    temp_v.insert(begin(temp_v), padding);
    temp_v.insert(begin(temp_v), padding);
    temp_v.insert(begin(temp_v), padding);
    temp_v.insert(begin(temp_v), padding);
    temp_v.insert(begin(temp_v), padding);
    temp_v.insert(begin(temp_v), padding);
    temp_v.push_back(padding);
    temp_v.push_back(padding);
    temp_v.push_back(padding);
    temp_v.push_back(padding);
    temp_v.push_back(padding);
    temp_v.push_back(padding);

    for (int r = 0; r < temp_v.size(); ++r)
    {
        for (int c = 0; c < temp_v[0].size(); ++c)
        {
            ump[make_pair(r, c)] = temp_v[r][c];
        }
        //cout << endl;
    }

    /*for (int r = 0; r < 5; ++r)
    {
        for (int c = 0; c < 5; ++c)
        {
            auto p = make_pair(r, c);
            cout << ump[p];
        }
        cout << endl;
    }*/
}

vector<pair<int, int>> generateNeighbors(int row, int col)
{
    vector<pair<int, int>> ret;

    auto one = make_pair(row - 1, col - 1);
    auto two = make_pair(row - 1, col);
    auto three = make_pair(row - 1, col + 1);

    auto four = make_pair(row, col - 1);
    auto five = make_pair(row, col);
    auto six = make_pair(row, col + 1);

    auto seven = make_pair(row + 1, col - 1);
    auto eight = make_pair(row + 1, col);
    auto nine = make_pair(row + 1, col + 1);

    ret.push_back(one);
    ret.push_back(two);
    ret.push_back(three);
    ret.push_back(four);
    ret.push_back(five);
    ret.push_back(six);
    ret.push_back(seven);
    ret.push_back(eight);
    ret.push_back(nine);

    return ret;
}

void getNextGeneration()
{
    unordered_map<pair<int, int>, char, hash_pair> new_pixels;
    for (auto &[k, v] : ump)
    {
        string key_string = "";
        auto neighbors = generateNeighbors(k.first, k.second);

        for (auto &n : neighbors)
        {
            if (ump.find(n) == ump.end())
            { // we are on a border
                if (v == '.')
                {
                    new_pixels[n] = '#'; // we need to expand into the void
                    key_string += '0';
                }
                else
                {
                    new_pixels[n] = '.';
                    key_string += '1';
                }
            }
            else
            {
                char blah = ump[n];
                if (blah == '.')
                {
                    key_string += '0';
                }
                else
                {
                    key_string += '1';
                }
            }
        }

        int ind = stoi(key_string, nullptr, 2);
        char nv = BIT_STRING[ind];
        new_pixels[k] = nv;
    }

    for (auto &[k, v] : new_pixels)
    {
        ump[k] = v;
    }
}

void getNumLight()
{
    int c = 0;
    for (auto &[k, v] : ump)
    {
        if (v == '#')
        {
            c++;
        }
    }
    cout << c;
    cout << endl;
}
void partOne()
{
    for (int i = 0; i < 50; ++i)
    {
        getNextGeneration();
    }

    getNumLight();
}

int main()
{
    parseInput();
    partOne();
}
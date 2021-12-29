#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <array>

//#include <eigen3/Eigen/Eigen> //if on work machine
#include <Eigen>
#include <set>
#include <sstream>
#include <string>
#include <sstream>
#include <cassert>
#include <utility>
#include <stack>
#include <limits>

// c++ -o day22.out -std=c++17 day22.cpp -I  /usr/local/Eigen

using namespace std;

using namespace Eigen;

using point_3d = tuple<int, int, int>;

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

template <typename T>
struct matrix_hash : std::unary_function<T, size_t>
{
    std::size_t operator()(T const &matrix) const
    {
        // Note that it is oblivious to the storage order of Eigen matrix (column- or
        // row-major). It will give you the same hash value for two different matrices if they
        // are the transpose of each other in different storage order.
        size_t seed = 0;
        for (size_t i = 0; i < matrix.size(); ++i)
        {
            auto elem = *(matrix.data() + i);
            seed ^= std::hash<typename T::Scalar>()(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct Block_
{
    Vector3i ub;
    Vector3i lb;

    bool isOn;
};
vector<Block_> adders;
vector<Block_> subtracters;
unordered_map<Vector3i, bool, matrix_hash<Vector3i>>
    cube_map;

int x_min = -50, x_max = 50;
int y_min = -50, y_max = 50;
int z_min = -50, z_max = 50;

void generateCubeMap()
{
    for (int i = x_min; i <= x_max; ++i)
    {
        for (int j = y_min; j <= y_max; ++j)
        {
            for (int k = z_min; k <= z_max; ++k)
            {
                Vector3i temp(i, j, k);
                cube_map[temp] = false;
            }
        }
    }
}

struct Instruction
{
    string on_off;
    pair<int, int> x_bounds;
    pair<int, int> y_bounds;
    pair<int, int> z_bounds;
    bool isValid;
};

vector<Instruction> ins_list;
void parseInput()
{

    ifstream f;
    f.open("day22.txt");

    string temp;
    while (getline(f, temp))
    {
        Instruction ins;
        stringstream ss(temp);
        string junk;
        ss >> junk;
        //cout << junk;
        assert(junk == "on" or junk == "off");
        ins.on_off = junk;

        string temp2;
        vector<pair<int, int>> asdf;
        while (getline(ss, temp2, ','))
        {
            //cout << temp2 << endl;
            int i = 0;
            while (!isdigit(temp2[i]) and temp2[i] != '-')
            {
                ++i;
            }
            temp2.erase(temp2.begin(), temp2.begin() + i);
            //cout << temp2 << endl;

            stringstream ss1(temp2);
            string temp3;
            int j = 0;
            pair<int, int> temp_p;
            while (getline(ss1, temp3, '.'))
            {

                //cout << temp3 << endl;
                if (!temp3.empty())
                {
                    if (j == 0)
                    {
                        temp_p.first = stoi(temp3);
                        ++j;
                    }
                    else
                    {
                        temp_p.second = stoi(temp3);
                    }
                }
            }
            asdf.push_back(temp_p);
        }
        /*for (auto &a : asdf)
        {
            cout << a.first << " " << a.second << endl;
        }*/
        ins.x_bounds = asdf[0];
        ins.y_bounds = asdf[1];
        ins.z_bounds = asdf[2];
        ins_list.push_back(ins);
    }

    for (auto &ins : ins_list)
    {
        cout << "On/off: " << ins.on_off << " \nxbounds: " << ins.x_bounds.first << " " << ins.x_bounds.second << endl;
        cout << "ybounds: " << ins.y_bounds.first << " " << ins.y_bounds.second << endl;
        cout << "zbounds: " << ins.z_bounds.first << " " << ins.z_bounds.second << endl;
        if (ins.x_bounds.second < x_min or ins.x_bounds.first > x_max or ins.y_bounds.second < y_min or ins.y_bounds.first > y_max or ins.z_bounds.second < z_min or ins.z_bounds.first > z_max)
        {
            ins.isValid = false;
        }
        else
        {
            ins.isValid = true;
        }
        cout << "is valid: " << ins.isValid << endl
             << endl;
    }
}

Block_ createBlock(Instruction &ins)
{
    Block_ temp;
    temp.lb = {ins.x_bounds.first, ins.y_bounds.first, ins.z_bounds.first};
    temp.ub = {ins.x_bounds.second, ins.y_bounds.second, ins.z_bounds.second};

    if (ins.on_off == "on")
    {
        temp.isOn = true;
    }
    else
    {
        temp.isOn = false;
    }
    return temp;
}

long long int getVolume(Block_ &blk)
{
    long long int x = blk.ub(0) - blk.lb(0) + 1;
    long long int y = blk.ub(1) - blk.lb(1) + 1;
    long long int z = blk.ub(2) - blk.lb(2) + 1;

    return x * y * z;
}

pair<Block_, bool> generateIntersection(Block_ &lhs, Block_ &rhs) // intersection block ALWAYS takes on state of lhs
{
    Block_ temp;

    auto x1_max = lhs.ub(0), y1_max = lhs.ub(1), z1_max = lhs.ub(2);
    auto x1_min = lhs.lb(0), y1_min = lhs.lb(1), z1_min = lhs.lb(2);
    auto x2_max = rhs.ub(0), y2_max = rhs.ub(1), z2_max = rhs.ub(2);
    auto x2_min = rhs.lb(0), y2_min = rhs.lb(1), z2_min = rhs.lb(2);

    long long int x_dim = max(min(x2_max, x1_max) - max(x1_min, x2_min), 0);
    if (min(x2_max, x1_max) - max(x1_min, x2_min) < 0)
    {
        x_dim = numeric_limits<long long int>::min();
    }
    long long int y_dim = max(min(y2_max, y1_max) - max(y1_min, y2_min), 0);
    if (min(y2_max, y1_max) - max(y1_min, y2_min) < 0)
    {
        y_dim = numeric_limits<long long int>::min();
    }
    long long int z_dim = max(min(z2_max, z1_max) - max(z1_min, z2_min), 0);
    if (min(z2_max, z1_max) - max(z1_min, z2_min) < 0)
    {
        z_dim = numeric_limits<long long int>::min();
    }

    if (x_dim < 0 or y_dim < 0 or z_dim < 0)
    {
        //cout << " No Intersection " << endl;
        return make_pair(temp, false);
    }

    Vector3i lb_int(max(x1_min, x2_min), max(y1_min, y2_min), max(z1_min, z2_min));
    Vector3i ub_int(min(x1_max, x2_max), min(y1_max, y2_max), min(z1_max, z2_max));

    temp.lb = lb_int;
    temp.ub = ub_int;

    temp.isOn = lhs.isOn;
    //max(min(a ',x') - max(a, x), 0) * max(min(b ',y') - max(b, y), 0) * max(min(c ',z') - max(c, z), 0);

    return make_pair(temp, true);
}

void partTwo()
{
    int i = 0;
    for (auto &ins : ins_list)
    {
        //cout << i << endl;
        auto curr = createBlock(ins);
        vector<Block_> add_inserters; // when we generate an intersection with a subtracter, we have to add it

        //cout << "Executed" << endl;
        if (i == 0)
        {
            adders.push_back(curr); // initialize the thing
            ++i;
            continue;
        }
        else
        {
            add_inserters.push_back(curr); // otherwise the current block can be an add inserter
        }
        //adders.push_back(curr);       // we always add the current one
        vector<Block_> sub_inserters; // when we generate an intersection with a adder, we have to subtract it

        for (auto &add_block : adders)
        {
            //cout << "Add Block Check" << endl;
            auto is_intersection = generateIntersection(add_block, curr);
            if (is_intersection.second) // if an intersection is found
            {
                //cout << "Found an Intersection in adders" << endl;
                sub_inserters.push_back(is_intersection.first); // we will need to subract this block later
            }
        }

        for (auto &sub_block : subtracters)
        {
            auto is_intersection = generateIntersection(sub_block, curr);
            if (is_intersection.second) // if an intersection is found
            {
                //cout << "Found an intersection in subbers";
                add_inserters.push_back(is_intersection.first); // we will need to subract this block later
            }
        }

        for (auto &sub : sub_inserters)
        {
            subtracters.push_back(sub);
        }
        for (auto &add : add_inserters)
        {
            adders.push_back(add);
        }
        i = i + 1;
    }

    long long int light_sum = 0;
    long long int dark_sum = 0;
    for (auto &ad : adders)
    {
        auto v = getVolume(ad);
        //cout << v << endl;
        if (ad.isOn)
        {
            light_sum += v;
        }
        else
        {
            dark_sum += v;
        }
    }
    for (auto &sub : subtracters)
    {
        auto v = getVolume(sub);
        if (sub.isOn)
        {
            light_sum -= v;
        }
        else
        {
            dark_sum -= v;
        }
    }

    cout << "Light Sum " << light_sum << endl;
    //cout << "Dark Sum" << dark_sum << endl;
}

void partOne()
{
    for (auto &ins : ins_list)
    {
        if (ins.isValid)
        {
            for (int i = ins.x_bounds.first; i <= ins.x_bounds.second; ++i)
            {
                for (int j = ins.y_bounds.first; j <= ins.y_bounds.second; ++j)
                {
                    for (int k = ins.z_bounds.first; k <= ins.z_bounds.second; ++k)
                    {
                        Vector3i temp(i, j, k);

                        if (ins.on_off == "on")
                        {
                            cube_map[temp] = true;
                        }
                        else
                        {
                            cube_map[temp] = false;
                        }
                    }
                }
            }
        }
    }
    int c = 0;
    for (auto &[elt, b] : cube_map)
    {
        if (b)
        {
            ++c;
        }
    }
    cout << endl;
    cout << c << endl;
}

int main()
{
    parseInput();
    //partOne();
    partTwo();
}
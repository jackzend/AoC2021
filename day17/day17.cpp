#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

// x = 236..262

int x_min = 236;
int x_max = 262;

int y_min = -78;
int y_max = -58;

using namespace std;

struct probe
{

    int vx;
    int vy;

    int x_pos;
    int y_pos;

    int y_max;

    bool isValid;

    bool inTheTarget;
};

vector<probe> velocity_and_pos; // curr velocities , curr positions

void partOne()
{

    for (int vx = 0; vx < 600; ++vx)
    {
        for (int vy = -600; vy < 600; ++vy)
        {
            probe temp;
            temp.isValid = true;
            temp.inTheTarget = true;
            temp.vx = vx;
            temp.vy = vy;
            temp.x_pos = 0;
            temp.y_pos = 0;
            temp.y_max = 0;

            velocity_and_pos.push_back(temp);
        }
    }

    bool someValid = true;
    while (someValid)
    {
        for (auto &p : velocity_and_pos)
        {
            if (p.isValid)
            {
                p.x_pos += p.vx;
                p.y_pos += p.vy;
                if (p.vx > 0)
                {
                    p.vx -= 1;
                }
                else if (p.vx < 0)
                {
                    p.vx += 1;
                }

                p.vy -= 1;

                if (p.y_pos > p.y_max)
                {
                    p.y_max = p.y_pos;
                }

                if (p.x_pos <= x_max and p.x_pos >= x_min and p.y_pos <= y_max and p.y_pos >= y_min)
                {
                    p.isValid = false;
                    p.inTheTarget = true;
                }
                else if (p.x_pos > x_max or p.y_pos < y_min)
                {
                    p.isValid = false;
                    p.inTheTarget = false;
                }
            }
        }
        someValid = false;
        for (auto &p : velocity_and_pos)
        {
            if (p.isValid)
            {
                someValid = true;
                break;
            }
        }
    }
    int max_num = 0;
    int count = 0;
    for (auto &p : velocity_and_pos)
    {
        if (p.inTheTarget)
        {
            ++count;
        }
        if (p.y_max > max_num and p.inTheTarget)
        {
            max_num = p.y_max;
        }
    }
    cout << "Max: " << max_num << " Count: " << count << endl;
}

int main()
{
    partOne();
}
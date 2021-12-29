#include <iostream>

using namespace std;

int p1_init = 7;
int p2_init = 3;

int p1_pos = p1_init;
int p2_pos = p2_init;

int p1_score = 0;
int p2_score = 0;

int die = 0;
int rolls = 0;

int rollOne()
{
    if (die == 100)
    {
        die = 0;
    }
    ++die;

    cout << "Die = " << die << ", ";

    ++rolls;

    return die;
}

int main()
{
    while (true)
    {
        int temp1 = /*p1_pos + */ rollOne() + rollOne() + rollOne();
        cout << endl;

        auto curr = p1_pos + temp1;
        /*for (int i = 0; i < temp1; ++i)
        {
            ++curr;
            if (curr == 11)
            {
                curr = 1;
            }
        }*/
        if (curr != 10)
        {
            curr = curr % 10;
        }
        p1_pos = curr;
        //cout << temp1 << endl;
        /*if (temp1 > 10)
        {
            p1_pos = temp1 % 10;
        }
        else
        {
            p1_pos = temp1;
        }*/

        p1_score += p1_pos;

        cout << "P1 Pos: " << p1_pos << " P1 Score: " << p1_score << endl;

        if (p1_score >= 1000)
        {
            cout << rolls * p2_score << endl;
            break;
        }

        int temp2 = /*p2_pos +*/ rollOne() + rollOne() + rollOne();
        cout << endl;

        curr = p2_pos + temp2;
        /*for (int i = 0; i < temp2; ++i)
        {
            ++curr;
            if (curr == 11)
            {
                curr = 1;
            }
        }*/
        if (curr != 10)
        {
            curr = curr % 10;
        }
        p2_pos = curr;
        /*if (temp2 > 10)
        {
            p2_pos = temp2 % 10;
        }
        else
        {
            p2_pos = temp2;
        }*/

        p2_score += p2_pos;

        cout << "P2 Pos: " << p2_pos << " P2 Score: " << p2_score << endl;
        if (p2_score >= 1000)
        {
            cout << rolls * p1_score << endl;
            break;
        }
    }
}
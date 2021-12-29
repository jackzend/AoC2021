#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cassert>
#include <utility>
#include <vector>
#include <set>
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

unordered_map<pair<int, int>, int, hash_pair> dieSums; // key is the sum, value is number of times it occurs at each split

unordered_map<pair<int, int>, unordered_map<pair<int, int>, long long int, hash_pair>, hash_pair> p_s_c_map;

void generateDieSums()
{
    int possibilites = 0;
    vector<int> p1_probabilities;
    for (int i = 1; i <= 3; ++i)
    {
        for (int j = 1; j <= 3; ++j)
        {
            for (int k = 1; k <= 3; ++k)
            {
                auto sum = i + j + k;
                p1_probabilities.push_back(sum);
            }
        }
    }

    for (int i = 0; i < p1_probabilities.size(); ++i)
    {
        for (int j = 0; j < p1_probabilities.size(); ++j)
        {
            auto temp = make_pair(p1_probabilities[i], p1_probabilities[j]);
            if (dieSums.find(temp) == dieSums.end())
            {
                dieSums[temp] = 1;
            }
            else
            {
                dieSums[temp] += 1;
            }
            ++possibilites;
        }
    }

    /*for (auto &[k, v] : dieSums)
    {
        cout << "P1 Sum: " << k.first << " P2 Sum: " << k.second << " Times Occuring: " << v << endl;
    }*/
    assert(possibilites == (27 * 27));
}

pair<int, int> getNewPosition(const pair<int, int> &lhs, const pair<int, int> &rhs)
{
    int newFirst = lhs.first + rhs.first;
    if (newFirst != 10)
    {
        newFirst = newFirst % 10;
    }
    int newSecond = lhs.second + rhs.second;
    if (newSecond != 10)
    {
        newSecond = newSecond % 10;
    }

    return make_pair(newFirst, newSecond);
}

pair<int, int> addPairs(const pair<int, int> &lhs, const pair<int, int> &rhs)
{
    return make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

long long int p1_wins = 0;
long long int p2_wins = 0;

int target = 21;

void gamePlay()
{

    p_s_c_map[make_pair(7, 3)][make_pair(0, 0)] = 1;

    //vector<pair<pair<int, int>, pair<int, int>>> deleter_scores;

    while (!p_s_c_map.empty())
    {
        vector<pair<pair<int, int>, unordered_map<pair<int, int>, long long int, hash_pair>>> inserters;
        vector<pair<int, int>> deleters;
        for (auto [player_position, score_map] : p_s_c_map)
        {
            for (auto &[ds, ds_count] : dieSums)
            {
                auto newPositionPair = getNewPosition(player_position, ds);
                unordered_map<pair<int, int>, long long int, hash_pair> newScoreMap;
                set<int> no_doubles;
                for (auto &[score, count] : score_map)
                {
                    auto newScorePair = addPairs(score, newPositionPair);
                    if (newScorePair.first >= target)
                    {

                        p1_wins += (count * static_cast<long long int>(ds_count));

                        continue;
                        //newScoreMap.erase(newScorePair);
                    }
                    else if (newScorePair.second >= target)
                    {
                        p2_wins += count * static_cast<long long int>(ds_count);
                        continue;
                        //newScoreMap.erase(newScorePair);
                    }
                    else
                    {
                        newScoreMap[newScorePair] = count * ds_count;
                    }
                }
                //cout << "New Score Map Size " << newScoreMap.size() << endl;
                if (!newScoreMap.empty())
                {
                    inserters.push_back(make_pair(newPositionPair, newScoreMap));
                }
            }
            deleters.push_back(player_position);
        }

        for (auto &del : deleters)
        {
            p_s_c_map.erase(del);
        }
        //cout << "INS Size: " << inserters.size() << endl;
        for (auto &ins : inserters)
        {
            if (p_s_c_map.find(ins.first) == p_s_c_map.end())
            {
                p_s_c_map[ins.first] = ins.second;
            }
            else
            {
                for (auto &[score, count] : ins.second)
                {
                    //cout << score.first << " " << score.second << endl;
                    assert(score.first < 21 and score.second < 21);
                    if (p_s_c_map[ins.first].find(score) == p_s_c_map[ins.first].end())
                    {
                        p_s_c_map[ins.first][score] = count;
                    }
                    else
                    {
                        p_s_c_map[ins.first][score] += count;
                    }
                }
            }
        }

        cout << "P1 Wins: " << p1_wins / 27 << " P2 Wins: " << p2_wins << endl;
    }

    // generate all possible new positions and insert those into position counts

    // generate all possible new scores and insert those into score counts

    // any moves that result in a winning score need to be removed from their thing
}
int main()
{
    generateDieSums();
    gamePlay();
}
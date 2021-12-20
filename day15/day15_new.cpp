#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>
#include <sstream>
#include <cmath>
#include <limits>
#include <cstddef>

using namespace std;

struct point
{
   int x;
   int y;
};
using PD = pair<double, point>;
struct hash_point
{
   size_t operator()( const point &p ) const
   {
      auto hash1 = hash<int>{} ( p.x );
      auto hash2 = hash<int>{} ( p.y );
      return hash1 ^ hash2;
   }
};

unordered_map<point, int, hash_point> coord_map;

unordered_map<point, point, hash_point> cameFrom;

unordered_map<point, double, hash_point> gScore;
unordered_map<point, double, hash_point> fScore;

priority_queue<PD, vector<PD>, greater<PD> > frontier;
set<point> frontier_s;

int MAX_ROW = 0;
int MAX_COL = 0;

void parse_input()
{
   ifstream f;
   f.open( "day15_test.txt" );

   string temp;
   int    row = 0;
   while ( getline( f, temp ) )
   {
      int col = 0;

      for ( auto c: temp )
      {
         point t_pair;
         t_pair.x          = col;
         t_pair.y          = row;
         coord_map[t_pair] = atoi( &c );
         cout << t_pair.x << "," << t_pair.y << ": " << coord_map[t_pair] << endl;
         ++col;
      }
      MAX_COL = col;
      ++row;
   }
   MAX_ROW = row;
}

int main()
{
   parse_input();
}

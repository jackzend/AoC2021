#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <set>
#include <algorithm>

using namespace std;

using rc_pair = pair<int, int>;

vector<vector<int> > heightmap;

vector<rc_pair> lowpoints;

void parse_input()
{
   ifstream f;
   f.open( "day8.txt" );

   string temp;

   while ( getline( f, temp ) )
   {
      vector<int> row;
      row.push_back( 10 );
      for ( int i = 0; i < temp.size(); ++i )
      {
         string c_s = "";
         c_s += temp[i];
         row.emplace_back( stoi( c_s ) );
      }
      row.push_back( 10 );
      heightmap.push_back( row );
   }
   vector<int> buff( heightmap[0].size(), 10 );

   heightmap.push_back( buff );
   heightmap.insert( begin( heightmap ), buff );

   /*for ( auto &row : heightmap )
   {
      for ( auto &elt : row )
      {
         cout << elt;
      }
      cout << endl;
   }
   */
}

bool isLowPoint( int i, int j )
{
   int p = heightmap[i][j];
   if ( p < heightmap[i + 1][j] and p < heightmap[i - 1][j] and p < heightmap[i][j + 1] and p < heightmap[i][j - 1] )
   {
      return true;
   }
   return false;
}

int partOne()
{
   int ret = 0;
   for ( int i = 1; i < heightmap.size() - 1; ++i )
   {
      for ( int j = 1; j < heightmap[0].size() - 1; ++j )
      {
         if ( isLowPoint( i, j ) )
         {
            cout << "Low Point Found: (" << i << "," << j << ")" << endl;
            int t = heightmap[i][j] + 1;
            cout << t << endl;
            ret += t;
            lowpoints.emplace_back( make_pair( i, j ) );
         }
      }
   }
   return ret;
}

vector<rc_pair> generateNeighbors( rc_pair &pt )
{
   vector<rc_pair> ret;
   ret.push_back( make_pair( pt.first - 1, pt.second ) );
   ret.push_back( make_pair( pt.first + 1, pt.second ) );
   ret.push_back( make_pair( pt.first, pt.second - 1 ) );
   ret.push_back( make_pair( pt.first, pt.second + 1 ) );
   return ret;
}

void partTwo()
{
   vector<int> basin_sizes;
   for ( auto &lp : lowpoints )
   {
      stack<rc_pair> frontier;
      set<rc_pair>   visited;
      frontier.push( lp );
      visited.insert( lp );

      int basin_count = 0;
      while ( !frontier.empty() )
      {
         auto current = frontier.top();
         frontier.pop();
         ++basin_count;

         auto temp_v = generateNeighbors( current );

         for ( auto &neighbor : temp_v )
         {
            if ( heightmap[neighbor.first][neighbor.second] < 9 and visited.find( neighbor ) == visited.end() )
            {
               visited.insert( neighbor );
               frontier.push( neighbor );
            }
         }
      }
      basin_sizes.push_back( basin_count );
   }
   sort( begin( basin_sizes ), end( basin_sizes ) );

   for ( auto &sz : basin_sizes )
   {
      cout << sz << " ";
   }
   cout << endl;

   cout << basin_sizes[basin_sizes.size() - 1] << " " << basin_sizes[basin_sizes.size() - 2] << " " << basin_sizes[basin_sizes.size() - 3] << endl;

}

int main()
{
   parse_input();

   partOne();
   partTwo();
}

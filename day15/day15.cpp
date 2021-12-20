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

struct hash_pair
{
   template<class T1, class T2>
   size_t operator()( const pair<T1, T2> &p ) const
   {
      auto hash1 = hash<T1>{} ( p.first );
      auto hash2 = hash<T2>{} ( p.second );
      return hash1 ^ hash2;
   }
};

unordered_map<pair<int, int>, int, hash_pair> coord_map;

unordered_map<pair<int, int>, pair<int, int>, hash_pair> cameFrom;

unordered_map<pair<int, int>, int, hash_pair>    gScore; // g(n), cost of cheapest path from start to n
unordered_map<pair<int, int>, double, hash_pair> fScore; // f(n) = g(n) + h(n) current best guess from start to finish thru n

int MAX_ROW = 0;                                         // 0 indexed
int MAX_COL = 0;                                         // 0 indexed

class Compare
{
   public:
      bool operator()( pair<int, int> &lhs, pair<int, int> &rhs )
      {
         return ( fScore[lhs] > fScore[rhs] );
      }
};
priority_queue<pair<int, int>, vector<pair<int, int> >, Compare> discovered;
set<pair<int, int> > discovered_s;

unordered_map<pair<int, int>, int, hash_pair> tile_map;

void fill_tile_map()
{
   tile_map[make_pair( 0, 0 )] = 0;

   for ( int i = 0; i < 5; ++i )
   {
      for ( int j = 0; j < 5; ++j )
      {
         if ( i == 0 and j == 0 )
         {
            /* do nothing */
         }
         else
         {
            if ( i == 0 )
            {
               tile_map[make_pair( i, j )] = tile_map[make_pair( i, j - 1 )] + 1;
            }
            else if ( j == 0 )
            {
               tile_map[make_pair( i, j )] = tile_map[make_pair( i - 1, j )] + 1;
            }
            else
            {
               tile_map[make_pair( i, j )] = tile_map[make_pair( i - 1, j )] + 1;
            }
         }

      }
   }

   /*for ( int i = 0; i < 5; ++i )
   {
      for ( int j = 0; j < 5; ++j )
      {
         cout << tile_map[make_pair( i, j )] << " ";
      }
      cout << endl;
   }*/
}

pair<int, int> getTileFromCoord( const pair<int, int> &coord )
{
   int row_tile = coord.first / MAX_ROW;
   int col_tile = coord.second / MAX_COL;

   auto ret = make_pair( row_tile, col_tile );
   return ret;
}

int getOriginalValue( const pair<int, int> &coord )
{
   int row_orig = coord.first % MAX_ROW;
   int col_orig = coord.second % MAX_COL;

   return coord_map[make_pair( row_orig, col_orig )];
}

int calc_value( pair<int, int> coord )
{
   //  cout << tile_map[getTileFromCoord( coord )] << endl;
   int ret = tile_map[getTileFromCoord( coord )] + getOriginalValue( coord );

   if ( ret >= 10 )
   {
      ret -= 9;
   }
   return ret;
}

void parse_input2()
{
   ifstream f;
   f.open( "day15.txt" );

   string temp;
   int    row = 0;
   while ( getline( f, temp ) )
   {
      int col = 0;

      for ( int i = 0; i < temp.size(); ++i )
      {
         pair<int, int> t_pair = make_pair( row, col );
         string c_s            = "";
         c_s              += temp[i];
         coord_map[t_pair] = stoi( c_s );
         //cout << t_pair.first << "," << t_pair.second << ": " << coord_map[t_pair] << endl;
         ++col;
      }
      MAX_COL = col;
      ++row;
   }
   MAX_ROW = row;                                  // LOL

   for ( int i = 0; i < MAX_ROW * 5; ++i )
   {
      for ( int j = 0; j < MAX_COL * 5; ++j )
      {
         coord_map[make_pair( i, j )] = calc_value( make_pair( i, j ) );
      }
   }

   /*vector<pair<pair<int, int>, int> > new_entries; //
   for ( auto & [k, v] : coord_map )
   {
      pair<int, int> n_d, n_r;
      for ( int i = 1; i <= 4; ++i )
      {
         int new_value = v + i;
         if ( new_value >= 10 )
         {
            new_value -= 9;
         }
         n_d.first  = k.first + ( i * MAX_ROW );
         n_d.second = k.second;
         //cout << v + i;
         new_entries.emplace_back( make_pair( n_d, new_value ) );
         //coord_map[n_d] = v + i;

         n_r.first  = k.first;
         n_r.second = k.second + ( i * MAX_COL );
         new_entries.emplace_back( make_pair( n_r, new_value ) );

         //         coord_map[n_r] = v + i;
      }

   }

   for ( auto &p : new_entries )
   {
      coord_map[p.first] = p.second;
   }*/

   /*for ( int i = 0; i < MAX_ROW * 5; ++i )
   {
      for ( int j = 0; j < MAX_COL * 5; ++j )
      {
         auto temp = make_pair( i, j );
         cout << coord_map[temp];
      }
      cout << endl;
   }*/

}

void parse_input()
{
   ifstream f;
   f.open( "day15_test.txt" );

   string temp;
   int    row = 0;
   while ( getline( f, temp ) )
   {
      int col = 0;

      for ( int i = 0; i < temp.size(); ++i )
      {
         pair<int, int> t_pair = make_pair( row, col );
         string c_s            = "";
         c_s              += temp[i];
         coord_map[t_pair] = stoi( c_s );
         //cout << t_pair.first << "," << t_pair.second << ": " << coord_map[t_pair] << endl;
         ++col;
      }
      MAX_COL = col;
      ++row;
   }
   MAX_ROW = row;
}

double h_manhattan( pair<int, int> &a, pair<int, int> &b )
{
   double x1 = static_cast<double>( a.first );
   double x2 = static_cast<double>( b.first );
   double y1 = static_cast<double>( a.second );
   double y2 = static_cast<double>( b.second );
   return abs( x1 - x2 ) + abs( y1 - y2 );

}

double h_eucl( pair<int, int> &a, pair<int, int> &b ) // euclidean distance
{
   double x1 = static_cast<double>( a.first );
   double x2 = static_cast<double>( b.first );
   double y1 = static_cast<double>( a.second );
   double y2 = static_cast<double>( b.second );
   return sqrt( pow( x1 - x2, 2 ) + pow( y1 - y2, 2 ) );

}

vector<pair<int, int> > gen_neighbors( pair<int, int> &curr )
{
   vector<pair<int, int> > ret;
   auto left  = make_pair( curr.first, curr.second - 1 );
   auto right = make_pair( curr.first, curr.second + 1 );
   auto up    = make_pair( curr.first + 1, curr.second );
   auto down  = make_pair( curr.first - 1, curr.second );

   ret.push_back( left );
   ret.push_back( right );
   ret.push_back( down );
   ret.push_back( up );

   return ret;
}
bool isValidNeighbor( pair<int, int> &p )
{
   if ( p.first >= 0 and p.first < MAX_ROW and p.second >= 0 and p.second < MAX_COL )
   {
      return true;
   }
   return false;

}

int reconstruct_path( pair<int, int> current )
{
   vector<pair<int, int> > total_path;

   total_path.push_back( current );
   while ( cameFrom.find( current ) != cameFrom.end() )
   {
      //cout << current.first << " " << current.second << endl;
      current = cameFrom[current];
      total_path.push_back( current );
   }

   int cost = 0;

   for ( auto &p : total_path )
   {
      //cout << coord_map[p] << endl;
      cost += coord_map[p];
   }
   //cout << cost << endl;
   return cost;
}

int AStar( pair<int, int> &start, pair<int, int> &goal )
{
   for ( auto & [k, v] : coord_map )
   {
      gScore[k] = numeric_limits<int>::max();
      fScore[k] = numeric_limits<double>::max();
   }
   fScore[start] = h_manhattan( start, goal );
   gScore[start] = 0;
   //cout << fScore[start];

   discovered.push( start );    // start at (0,0)
   discovered_s.insert( start );

   while ( !discovered.empty() )
   {
      pair<int, int> current = discovered.top();
      if ( current == goal )
      {
         return reconstruct_path( current ) - coord_map[start];
      }

      discovered.pop();
      discovered_s.erase( current );

      auto temp_v = gen_neighbors( current );
      for ( auto &p : temp_v )
      {
         if ( isValidNeighbor( p ) )
         {
            int temp_gScore = gScore[current] + coord_map[p];
            //cout << temp_gScore << endl;
            if ( temp_gScore < gScore[p] )
            {
               cameFrom[p] = current;
               gScore[p]   = temp_gScore;
               fScore[p]   = static_cast<double>( temp_gScore ) + h_manhattan( p, goal );
               if ( discovered_s.find( p ) == discovered_s.end() )
               {
                  //cout << p.first << " " << p.second << endl;
                  discovered_s.insert( p );
                  discovered.emplace( p );
               }
            }
         }
      }
   }
   return 0;

}

int main()
{
   //cout << numeric_limits<int>::max() << endl;
   fill_tile_map();
   parse_input2();

   //parse_input2();
   MAX_COL *= 5;
   MAX_ROW *= 5;
   pair<int, int> start = make_pair( 0, 0 );
   //cout << MAX_ROW << " " << MAX_COL;
   pair<int, int> goal = make_pair( MAX_ROW - 1, MAX_COL - 1 );
   cout << AStar( start, goal ) << endl;
}

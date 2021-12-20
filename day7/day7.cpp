#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

vector<int> v;

int part1()
{
   auto mid = begin( v );
   advance( mid, v.size() / 2 );
   nth_element( begin( v ), mid, end( v ) );

   //cout << *mid << endl;

   int tot = 0;
   for ( auto i : v )
   {
      tot += abs( i - *mid );
   }
   return tot;
}

int cost( int a, int goal )
{
   if ( a == goal ) { return 0; }
   int sum = 0;
   for ( int i = 0; i < abs( goal - a ); ++i )
   {
      sum += ( i + 1 );
   }
   return sum;
}

int part2()
{
   sort( begin( v ), end( v ) );
   int min_cost = RAND_MAX;
   //cout << min_cost << endl;
   for ( int i = 0; i < *max_element( v.begin(), v.end() ); ++i )
   {
      int tot_cost = 0;
      for ( auto &j : v )
      {
         tot_cost += cost( j, i );
      }
      //cout << tot_cost << endl;
      if ( tot_cost < min_cost )
      {
         min_cost = tot_cost;
      }
   }

   return min_cost;
}

int main()
{
   ifstream f;
   f.open( "day7.txt" );

   string temp;
   while ( getline( f, temp, ',' ) )
   {
      int t = stoi( temp );
      v.push_back( t );
   }

   //cout << part1();

   cout << part2();
}

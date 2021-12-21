#include <vector>
#include <iostream>
#include <utility>
#include <set>
#include <algorithm>

using namespace std;

using point_2d = pair<int, int>;
using point_3d = tuple<int, int, int>;

set<pair<int, int> > computeRelativePositionSet( vector<point_2d> &scanner )
{
   set<pair<int, int> > ret;
   for ( int i = 0; i < scanner.size(); ++i )
   {
      for ( int j = 0; j < scanner.size(); ++j )
      {
         if ( i != j )
         {
            auto b1  = scanner[i];
            auto b2  = scanner[j];
            auto rel = make_pair( b2.first - b1.first, b2.second - b1.second );
            ret.insert( rel );
         }
      }
   }
   return ret;
}

bool foundMatch_noRotations( vector<point_2d> &s1, vector<point_2d> &s2 )
{
   auto scanner1RelSet = computeRelativePositionSet( s1 );
   auto scanner2RelSet = computeRelativePositionSet( s2 );

   for ( auto it : scanner1RelSet )
   {
      cout << it.first << " " << it.second << endl;
   }
   cout << endl;
   for ( auto it : scanner2RelSet )
   {
      cout << it.first << " " << it.second << endl;
   }
   cout << endl;
   vector<pair<int, int> > v_out( scanner1RelSet.size() + scanner2RelSet.size() );

   auto it = set_intersection( scanner1RelSet.begin(),
                               scanner1RelSet.end(),
                               scanner2RelSet.begin(),
                               scanner2RelSet.end(),
                               v_out.begin() );
   v_out.resize( it - v_out.begin() );

   for ( auto it : v_out )
   {
      cout << it.first << " " << it.second << endl;
   }

   if ( v_out.size() >= 6 )
   {
      return true;
   }
   return false;

}

int main()
{
   vector<point_2d> scanner1;

   point_2d s1b1( 0, 2 );
   point_2d s1b2( 4, 1 );
   point_2d s1b3( 3, 3 );

   scanner1.push_back( s1b1 );
   scanner1.push_back( s1b2 );
   scanner1.push_back( s1b3 );

   vector<point_2d> scanner2;
   point_2d s2b1( -1, -1 );
   point_2d s2b2( -5, 0 );
   point_2d s2b3( -2, 1 );

   scanner2.push_back( s2b1 );
   scanner2.push_back( s2b2 );
   scanner2.push_back( s2b3 );

}

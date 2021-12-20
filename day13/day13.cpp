#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <utility>
#include <set>
#include <list>
#include <limits>

using namespace std;

using point     = pair<int, int>;
using direction = pair<char, int>;

vector<direction> dirs_l; // direction list;
set<point> ps;            // point set

void parseInput()
{
   ifstream f;
   f.open( "day13.txt" );

   string temp;
   int    f_i, s_i;
   char   junk;
   while ( true )
   {
      getline( f, temp );
      if ( temp.empty() ) { break; }
      stringstream ss( temp );

      ss >> f_i >> junk >> s_i;
      ps.emplace( make_pair( f_i, s_i ) );
      //cout << f_i << " : " << s_i << endl;
   }

   string dir, junk_s;
   int    num;
   while ( getline( f, temp ) )
   {
      stringstream ss( temp );
      ss >> junk_s >> junk_s >> dir;
      //cout << dir << endl;
      char xy = dir[0];
      dir.erase( begin( dir ), begin( dir ) + 2 );
      int num = stoi( dir );

      dirs_l.emplace_back( make_pair( xy, num ) );

      //cout << xy << " " << num << endl;
   }
   for ( auto &it : dirs_l )
   {
      //cout << it.first << " : " << it.second << endl;
   }
}

point newPoint( const point &old, const direction &dir )
{
   char  xy = dir.first;
   point newP;
   if ( xy == 'y' )
   {
      if ( old.second < dir.second )
      {
         newP = old;
      }
      else
      {
         int dist = old.second - dir.second;
         int newY = dir.second - dist;
         newP = make_pair( old.first, newY );
      }
   }
   else
   {
      if ( old.first < dir.second )
      {
         newP = old;
      }
      else
      {
         int dist = old.first - dir.second;
         int newX = dir.second - dist;
         newP = make_pair( newX, old.second );
      }
   }
   return newP;
}

void partOne()
{
   auto first_dir = dirs_l[0];
   vector<point> erase_pts;
   vector<point> ins_pts;
   for ( auto &pt: ps )
   {
      auto newP = newPoint( pt, first_dir );
      if ( newP == pt )
      {
         // these are the same do nothing
      }
      else // these points are different
      {
         erase_pts.push_back( pt ); // old point go bye bye
         ins_pts.push_back( newP ); // in comes the new points
      }
   }
   for ( auto &pt : erase_pts )
   {
      ps.erase( pt );
   }
   for ( auto &pt : ins_pts )
   {
      ps.insert( pt );
   }
   cout << ps.size() << endl;
}
void partTwo()
{
   for ( auto &first_dir : dirs_l )
   {
      vector<point> erase_pts;
      vector<point> ins_pts;
      for ( auto &pt: ps )
      {
         auto newP = newPoint( pt, first_dir );
         if ( newP == pt )
         {
            // these are the same do nothing
         }
         else // these points are different
         {
            erase_pts.push_back( pt ); // old point go bye bye
            ins_pts.push_back( newP ); // in comes the new points
         }
      }
      for ( auto &pt : erase_pts )
      {
         ps.erase( pt );
      }
      for ( auto &pt : ins_pts )
      {
         ps.insert( pt );
      }
      int min_x = numeric_limits<int>::max(), max_x = -1, min_y = numeric_limits<int>::max(), max_y = -1;
      for ( auto &pt : ps )
      {
         if ( pt.first > max_x )
         {
            max_x = pt.first;
         }
         else if ( pt.first < min_x )
         {
            min_x = pt.first;
         }
         if ( pt.second > max_y )
         {
            max_y = pt.second;
         }
         else if ( pt.second < min_y )
         {
            min_y = pt.second;
         }
      }

      //cout << min_y << " " << max_y << " " << min_x << " " << max_x;

      vector<char> row( max_x + 1, '.' );
      vector<vector<char> > c_v( max_y + 1, row );

      for ( auto &pt: ps )
      {
         int row = pt.second;
         int col = pt.first;

         c_v[row][col] = '#';
      }

      for ( auto &row : c_v )
      {
         for ( auto &c : row )
         {
            cout << c;
         }
         cout << endl;
      }
      cout << endl;
   }
   // cout << ps.size() << endl;

}

int main()
{
   parseInput();
   //partOne();
   partTwo();
}

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <unordered_map>

using namespace std;

/** INDEXING the Thing
 *
 *
 *          - 0 -
 *          1   2
 *          - 3 -
 *          4   5
 *          - 6 -
 *
 *
 */

set<char> ONE { '2', '5' };
set<char> TWO { '0', '2', '' }

struct sample
{
   vector<string>                     signals;
   vector<string>                     outputs;
   set<char>                          semicertain;   // set of chars we are semi certain about
   unordered_map<char, vector<char> > certainty_map; // mapping of an index (see above) to strings
};

vector<sample> s_v; // sample vector

void parseInput()
{
   ifstream f;
   f.open( "day8.txt" );

   string temp;
   while ( getline( f, temp ) )
   {
      stringstream ss( temp );
      sample s;
      string t;
      for ( int i = 0; i < 10; ++i )
      {
         ss >> t;
         s.signals.push_back( t );
      }
      ss >> t; // "|"

      for ( int i = 0; i < 4; ++i )
      {
         ss >> t;
         s.outputs.push_back( t );
      }
      s_v.push_back( s );
   }

}

void partOne()
{
   int u_c = 0;
   for ( auto &s : s_v )
   {
      for ( auto &out : s.outputs )
      {
         int sz = out.size();
         if ( sz == 2 or sz == 4 or sz == 3 or sz == 7 )
         {
            u_c++;
         }
      }
   }
   cout << u_c << endl;
}
void partTwo() {}

int main()
{
   parseInput();
   partOne();
   partTwo();
}

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
int main()
{
   ifstream f;
   f.open( "day1_input.txt" );
   vector<int> v;
   int temp;
   while ( f >> temp )
   {
      v.push_back( temp );
   }
   f.close();

   int c = 0;

   for ( size_t i = 1; i < v.size(); ++i )
   {
      if ( v[i] > v[i - 1] )
      {
         ++c;
      }
   }

   int c1 = 0;
   int first_sum, second_sum;
   for ( size_t i = 3; i < v.size(); ++i )
   {
      first_sum  = v[i] + v[i - 1] + v[i - 2];
      second_sum = v[i - 1] + v[i - 2] + v[i - 3];

      if ( first_sum > second_sum )
      {
         ++c1;
      }
   }

   cout << c << endl << c1;
   return 0;
}

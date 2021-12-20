#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <numeric>

using namespace std;

unordered_map<int, long long int> timer_counts;

int main()
{
   ifstream f;
   f.open( "day6.txt" );

   string temp_s;
   for ( int i = 0; i <= 9; ++i )
   {
      timer_counts[i] = 0;
   }

   while ( getline( f, temp_s, ',' ) )
   {
      int t = stoi( temp_s );
      timer_counts[t] += 1;
   }

   for ( int i = 0; i < 256; ++i )
   {
      long long int temp;

      for ( int j = 0; j <= 8; ++j ) // iterate through keys
      {
         if ( j == 0 )
         {
            temp = timer_counts[j];
         }
         else
         {
            timer_counts[j - 1] = timer_counts[j];
         }
         timer_counts[j] = 0;
      }
      timer_counts[8]  = temp;
      timer_counts[6] += temp;
   }
   long long int init = 0;

   for ( auto & [k, v] : timer_counts )
   {
      init += v;
   }
   cout << init;

}

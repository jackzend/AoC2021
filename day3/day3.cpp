#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

struct Binary_String
{
   string str;
   bool   isDeleted;
};

int main()
{
   ifstream f;
   f.open( "day3.txt" );

   vector<Binary_String> bin_strs;  // pair is the binary string and a flag: isDeleted

   vector<int> most_common_v;

   string tmp;
   size_t bit_length;
   size_t first_count = 0;
   while ( f >> tmp )
   {
      if ( first_count == 0 )
      {
         bit_length  = tmp.length();
         first_count = 1;
      }
      Binary_String tmp1;
      tmp1.str       = tmp;
      tmp1.isDeleted = false;
      bin_strs.emplace_back( tmp1 );
   }

   size_t size = bin_strs.size();
   for ( size_t i = 0; i < bit_length; ++i )
   {
      int    one_count = 0;
      size_t del_count = 0;

      for ( auto &bin : bin_strs )
      {
         if ( bin.str[i] == '1' and !bin.isDeleted )
         {
            ++one_count;
         }
      }

      double sz_d  = static_cast<double>( size );
      double one_d = static_cast<double>( one_count );

      int most_common = round( one_d / sz_d );

      int zero_num = size - one_count;

      if ( zero_num == one_count )
      {
         cout << "blah ";
         most_common = 1;
      }

      /*if ( most_common == 1 ) // flip it
      {
         most_common = 0;
      }
      else
      {
         most_common = 1;
      }*/
      most_common_v.push_back( most_common );

      for ( auto &bin : bin_strs )
      {
         if ( !bin.isDeleted )
         {
            if ( most_common == 1 )
            {
               if ( bin.str[i] == '0' )
               {
                  bin.isDeleted = true;
                  ++del_count;
               }
            }
            else
            {
               if ( bin.str[i] == '1' )
               {
                  bin.isDeleted = true;
                  ++del_count;
               }
            }
         }
      }
      size -= del_count;
      if ( size == 1 )
      {
         for ( auto &it : bin_strs )
         {
            if ( !it.isDeleted )
            {
               cout << it.str;
            }
         }
         break;
      }

   }
   /*for ( auto &it : most_common_v )
   {
      cout << it << " ";
   }*/

}

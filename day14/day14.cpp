#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <limits>

using namespace std;

using char_p = pair<char, char>;

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

unordered_map<char_p, char, hash_pair> ins_mp; // insertion map

string init;

void parseInput()
{
   ifstream f;
   f.open( "day14.txt" );

   f >> init;
   //cout << init << endl;
   string p_rule, junk;
   char   ins;
   while ( f >> p_rule >> junk >> ins )
   {
      //cout << p_rule << " : " << ins << endl;

      char_p tp = make_pair( p_rule[0], p_rule[1] );

      ins_mp[tp] = ins;
   }
   /*for ( auto & [k, v] :  ins_mp )
   {
      cout << k.first << k.second << " : " << v << endl;
   }*/
}

void partOne()
{
   unordered_map<char_p, long long int, hash_pair> pair_counts;
   unordered_map<char, long long int> char_counts;

   for ( int i = 0; i < init.size(); ++i )
   {
      if ( char_counts.find( init[i] ) == char_counts.end() )
      {
         char_counts[init[i]] = 1;
      }
      else
      {
         char_counts[init[i]] += 1;
      }
   }

   for ( int i = 0, j = 1; j < init.size(); ++i, ++j )
   {
      char_p tp = make_pair( init[i], init[j] );
      if ( pair_counts.find( tp ) == pair_counts.end() )
      {
         pair_counts[tp] = 1;
      }
      else
      {
         pair_counts[tp] += 1;
      }
   }

   for ( int i = 0; i < 40; ++i )
   {
      vector<char_p> del_v;                       // vector of what we are deleting
      vector<pair<char_p, long long int> > ins_v; // vector of what we are inserting (and how much)
      for ( auto & [k, v] : pair_counts )
      {
         long long int new_count = v;
         char inserter           = ins_mp[k]; // we just created a new char, keep track of it

         if ( char_counts.find( inserter ) == char_counts.end() )
         {
            char_counts[inserter] = v;
         }
         else
         {
            char_counts[inserter] += v;
         }
         auto new_pair1 = make_pair( k.first, inserter );
         auto new_pair2 = make_pair( inserter, k.second );

         ins_v.emplace_back( make_pair( new_pair1, new_count ) );
         ins_v.emplace_back( make_pair( new_pair2, new_count ) );
         del_v.push_back( k );
      }

      for ( auto &p : del_v )
      {
         pair_counts.erase( p );
      }
      for ( auto &p : ins_v )
      {
         if ( pair_counts.find( p.first ) == pair_counts.end() )
         {
            pair_counts[p.first] = p.second;
         }
         else
         {
            pair_counts[p.first] += p.second;
         }
      }
      for ( auto & [k, v] :  pair_counts )
      {
         cout << k.first << k.second << " : " << v << endl;
      }
      cout << endl;
   }

   long long int most_common  = 0;
   long long int least_common = numeric_limits<long long int>::max();
   for ( auto & [k, v] : char_counts )
   {
      if ( v > most_common )
      {
         most_common = v;
      }
      if ( v < least_common )
      {
         least_common = v;
      }
      cout << k << " :: " << v << endl;
   }
   cout << most_common - least_common << endl;

}
void partTwo() {}

int main()
{
   parseInput();
   partOne();
   partTwo();
}

#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <stack>
#include <set>
using namespace std;

using edge_v = vector<string>;

unordered_map<string, edge_v> e_mp;

bool isBig( string &node )
{
   if ( isupper( node[0] ) )
   {
      return true;
   }
   return false;
}

void parseInput()
{
   ifstream f;
   f.open( "day12.txt" );
   string temp;
   while ( getline( f, temp ) )
   {
      string id1 = "";
      string id2 = "";
      int    i;
      for ( i = 0; i < temp.size(); ++i )
      {
         if ( temp[i] == '-' )
         {
            ++i;
            break;
         }
         id1 += temp[i];
      }

      if ( e_mp.find( id1 ) == e_mp.end() ) // havent seen it yet
      {
         edge_v t;
         e_mp[id1] = t;
      }

      for ( i; i < temp.size(); ++i )
      {
         id2 += temp[i];
      }

      if ( e_mp.find( id2 ) == e_mp.end() )     // havent seen it yet
      {
         edge_v t;
         e_mp[id2] = t;
      }
      // insert two way edges
      e_mp[id2].push_back( id1 );
      e_mp[id1].push_back( id2 );
   }

   for ( auto & [k, v] : e_mp )
   {
      cout << k << " : ";
      for ( auto &i : v )
      {
         cout << i << ",";
      }
      cout << endl;
   }
}

void partOne()
{
   stack<vector<string> > edge_stack;
   vector<string> init;
   init.push_back( "start" );
   edge_stack.push( init );
   int count = 0;

   while ( !edge_stack.empty() )
   {
      vector<string> t_path = edge_stack.top();
      edge_stack.pop();

      if ( t_path[t_path.size() - 1] == "end" )
      {
         ++count;
         continue;
      }

      for ( auto &nb : e_mp[t_path[t_path.size() - 1]] )
      {
         bool hasValid_neighbor = false;
         vector<string> temp    = t_path;
         if ( isBig( nb ) )
         {
            temp.push_back( nb );
            hasValid_neighbor = true;
         }
         else
         {
            bool found = false;
            for ( auto &n : temp )
            {
               if ( n == nb )
               {
                  found = true;
                  break;
               }
            }
            if ( !found )
            {
               temp.push_back( nb );
               hasValid_neighbor = true;
            }
         }
         if ( hasValid_neighbor )
         {
            edge_stack.push( temp ); // push new paths
         }
      }

   }
   cout << endl << count << endl;
}

void partTwo()
{
   stack<vector<string> > edge_stack;
   vector<string> init;
   init.push_back( "start" );
   edge_stack.push( init );
   int count = 0;

   while ( !edge_stack.empty() )
   {
      vector<string> t_path = edge_stack.top();
      edge_stack.pop();

      if ( t_path[t_path.size() - 1] == "end" )
      {
         for ( auto &n : t_path )
         {
            cout << n << " ";
         }
         cout << endl;
         ++count;
         continue;
      }

      for ( auto &nb : e_mp[t_path[t_path.size() - 1]] )
      {
         if ( nb == "start" ) { continue; } // start is not a valid neighbor
         bool hasValid_neighbor = false;
         vector<string> temp    = t_path;
         if ( isBig( nb ) )
         {
            temp.push_back( nb );
            hasValid_neighbor = true;
         }
         else
         {
            bool found2         = false;
            bool found1_and_ins = false;
            int  found_c        = 0;
            for ( int i = 1; i < temp.size(); ++i )
            {
               auto n = temp[i];
               if ( n == nb )
               {
                  ++found_c;
                  if ( found_c == 2 )
                  {
                     found2 = true;
                     break;
                  }
               }
            }
            if ( !found2 )
            {
               if ( found_c == 1 )
               {
                  temp.insert( begin( temp ), "BLAH" );
               }
               temp.push_back( nb );
               hasValid_neighbor = true;
            }
         }
         if ( hasValid_neighbor )
         {
            if ( temp[1] != "BLAH" )    // prune branches with two BLAHs lol
            {
               edge_stack.push( temp ); // push new paths
            }
         }
      }

   }
   cout << endl << count << endl;
}

int main()
{
   parseInput();
   partOne();
   partTwo();
}

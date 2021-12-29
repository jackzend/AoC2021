#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>
#include <cmath>
#include <cassert>
#include <queue>
#include <limits>
#include <stack>
#include <unordered_set>
using namespace std;

int NUM_COL = 0;
int NUM_ROW = 0;

class game_state
{
   public:

      game_state()
      {
         vector<char> hw( 11, '.' );
         hallway = hw;

         a_room = make_pair( '.', '.' );
         b_room = make_pair( '.', '.' );
         c_room = make_pair( '.', '.' );
         d_room = make_pair( '.', '.' );
      }
      vector<char> hallway;

      pair<char, char> a_room;
      pair<char, char> b_room;
      pair<char, char> c_room;
      pair<char, char> d_room;

      int energy = 0;

}; // needs to be hashable

inline bool operator==( const game_state &lhs, const game_state &rhs )
{
   return ( lhs.hallway == rhs.hallway and lhs.a_room == rhs.a_room and lhs.b_room == rhs.b_room and lhs.c_room == rhs.c_room and lhs.d_room == rhs.d_room );
}

inline bool operator!=( const game_state &lhs, const game_state &rhs )
{
   return !( lhs == rhs );
}

vector<char> flattenGameState( const game_state &gs )
{
   vector<char> flat = gs.hallway;
   flat.push_back( gs.a_room.first );
   flat.push_back( gs.a_room.second );
   flat.push_back( gs.b_room.first );
   flat.push_back( gs.b_room.second );
   flat.push_back( gs.c_room.first );
   flat.push_back( gs.c_room.second );
   flat.push_back( gs.d_room.first );
   flat.push_back( gs.d_room.second );

   return flat;
}
struct game_state_hasher
{
   std::size_t operator()( game_state const &gs ) const
   {
      auto flattened   = flattenGameState( gs );
      std::size_t seed = flattened.size();
      for ( auto &i : flattened )
      {
         size_t s = static_cast<size_t>( i );
         seed ^= s + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
      }
      return seed;
   }
};

class Compare
{
   public:
      bool operator()( game_state &lhs, game_state &rhs )
      {
         return ( lhs.energy > lhs.energy );
      }
};

game_state INIT;

void printGameState( game_state &gs )
{
   assert( gs.hallway.size() == 11 );
   for ( auto &pos : gs.hallway )
   {
      cout << pos;
   }
   cout << endl;
   cout << "Room A: " << gs.a_room.first << gs.a_room.second << endl;
   cout << "Room B: " << gs.b_room.first << gs.b_room.second << endl;
   cout << "Room C: " << gs.c_room.first << gs.c_room.second << endl;
   cout << "Room D: " << gs.d_room.first << gs.d_room.second << endl;

   cout << "Energy: " << gs.energy << endl;

}

void parseInput( string &fn )
{
   assert( INIT.energy == 0 );
   ifstream f;
   f.open( fn );
   string temp;
   getline( f, temp ); // junk line

   getline( f, temp );
   for ( int i = 1; i < temp.size() - 1; ++i )
   {
      INIT.hallway[i] = temp[i];
   }

   getline( f, temp );
   INIT.a_room.first = temp[3];
   INIT.b_room.first = temp[5];
   INIT.c_room.first = temp[7];
   INIT.d_room.first = temp[9];

   getline( f, temp );

   INIT.a_room.second = temp[3];
   INIT.b_room.second = temp[5];
   INIT.c_room.second = temp[7];
   INIT.d_room.second = temp[9];

   printGameState( INIT );
}

game_state getGoal()
{
   game_state goal_state;
   goal_state.a_room.first  = 'A';
   goal_state.a_room.second = 'A';
   goal_state.b_room.first  = 'B';
   goal_state.b_room.second = 'B';
   goal_state.c_room.first  = 'C';
   goal_state.c_room.second = 'C';
   goal_state.d_room.first  = 'D';
   goal_state.d_room.second = 'D';
   return goal_state;
}

bool empty_chars( vector<char> &v )
{
   for ( auto &c : v )
   {
      if ( c != '.' )
      {
         return false;
      }
   }
   return true;
}

bool empty_chars( pair<char, char> &p )
{
   return p.first == '.' and p.second == '.';
}

vector<game_state> generate_neighbors( game_state &curr )
{
   cout << "In Generate Neighbors\n";
   vector<game_state> ret;

   if ( empty_chars( curr.hallway ) )
   {
      // room_a
      if ( !( curr.a_room.first == 'A' and curr.a_room.second == 'A' ) )
      {
         auto first_out_a = curr.a_room.first;
         auto new_a_room  = make_pair( '.', curr.a_room.second );
         for ( int i = 0; i < 11; ++i )
         {
            if ( i != 2 and i != 4 and i != 6 and i != 8 )
            {
               game_state new_gs = curr;
               new_gs.hallway[i] = first_out_a;
               new_gs.a_room     = new_a_room;
               ret.push_back( new_gs );
               //printGameState( new_gs );
            }
         }
      }
      // room_b
      if ( !( curr.b_room.first == 'B' and curr.b_room.second == 'B' ) )
      {
         auto first_out_b = curr.b_room.first;
         auto new_b_room  = make_pair( '.', curr.b_room.second );
         for ( int i = 0; i < 11; ++i )
         {
            if ( i != 2 and i != 4 and i != 6 and i != 8 )
            {
               game_state new_gs = curr;
               new_gs.hallway[i] = first_out_b;
               new_gs.b_room     = new_b_room;
               ret.push_back( new_gs );
               //printGameState( new_gs );
            }
         }
      }
      // room_c
      if ( !( curr.c_room.first == 'C' and curr.c_room.second == 'C' ) )
      {
         auto first_out_c = curr.c_room.first;
         auto new_c_room  = make_pair( '.', curr.c_room.second );
         for ( int i = 0; i < 11; ++i )
         {
            if ( i != 2 and i != 4 and i != 6 and i != 8 )
            {
               game_state new_gs = curr;
               new_gs.hallway[i] = first_out_c;
               new_gs.c_room     = new_c_room;
               ret.push_back( new_gs );
               //printGameState( new_gs );
            }
         }
      }
      // room_d
      if ( !( curr.d_room.first == 'D' and curr.d_room.second == 'D' ) )
      {
         auto first_out_d = curr.d_room.first;
         auto new_d_room  = make_pair( '.', curr.d_room.second );
         for ( int i = 0; i < 11; ++i )
         {
            if ( i != 2 and i != 4 and i != 6 and i != 8 )
            {
               game_state new_gs = curr;
               new_gs.hallway[i] = first_out_d;
               new_gs.d_room     = new_d_room;
               ret.push_back( new_gs );
               //printGameState( new_gs );
            }
         }
      }
   }
   else // hall way is not empty
   {
      // generate all the cases for the rooms
      if ( !empty_chars( curr.a_room ) and curr.a_room.second != 'A' ) // case for room a
      {
         pair<char, char> new_a_room;
         char first_out_a;
         if ( curr.a_room.first == '.' )
         {
            new_a_room  = make_pair( '.', '.' );
            first_out_a = curr.a_room.second;
         }
         else
         {
            new_a_room  = make_pair( '.', curr.a_room.second );
            first_out_a = curr.a_room.first;
         }

         int l = 2;                      // we want a range
         int r = 2;                      // two pointer approach to get the span
         while ( curr.hallway[r] == '.' )
         {
            ++r;
         }
         while ( curr.hallway[l] == '.' )
         {
            --l;
         }
         ++l;
         for ( int i = l; i < r; ++i )
         {
            if ( i != 2 and i != 4 and i != 6 and i != 8 )
            {
               game_state new_gs = curr;
               new_gs.hallway[i] = first_out_a;
               new_gs.a_room     = new_a_room;
               ret.push_back( new_gs );
               printGameState( new_gs );
            }
         }
      }

      // generate all the cases for the rooms
      if ( !empty_chars( curr.b_room ) and curr.b_room.second != 'B' )    // case for room a
      {
         pair<char, char> new_b_room;
         char first_out_b;
         if ( curr.b_room.first == '.' )
         {
            new_b_room  = make_pair( '.', '.' );
            first_out_b = curr.b_room.second;
         }
         else
         {
            new_b_room  = make_pair( '.', curr.b_room.second );
            first_out_b = curr.b_room.first;
         }

         int l = 2;                      // we want a range
         int r = 2;                      // two pointer approach to get the span
         while ( curr.hallway[r] == '.' )
         {
            ++r;
         }
         while ( curr.hallway[l] == '.' )
         {
            --l;
         }
         ++l;
         for ( int i = l; i < r; ++i )
         {
            if ( i != 2 and i != 4 and i != 6 and i != 8 )
            {
               game_state new_gs = curr;
               new_gs.hallway[i] = first_out_b;
               new_gs.b_room     = new_b_room;
               ret.push_back( new_gs );
               printGameState( new_gs );
            }
         }

      }

      // generate all the cases for the rooms
      if ( !empty_chars( curr.c_room ) and curr.c_room.second != 'C' )    // case for room a
      {
         pair<char, char> new_c_room;
         char first_out_c;
         if ( curr.c_room.first == '.' )
         {
            new_c_room  = make_pair( '.', '.' );
            first_out_c = curr.c_room.second;
         }
         else
         {
            new_c_room  = make_pair( '.', curr.c_room.second );
            first_out_c = curr.c_room.first;
         }

         int l = 2;                      // we want a range
         int r = 2;                      // two pointer approach to get the span
         while ( curr.hallway[r] == '.' )
         {
            ++r;
         }
         while ( curr.hallway[l] == '.' )
         {
            --l;
         }
         ++l;
         for ( int i = l; i < r; ++i )
         {
            if ( i != 2 and i != 4 and i != 6 and i != 8 )
            {
               game_state new_gs = curr;
               new_gs.hallway[i] = first_out_c;
               new_gs.c_room     = new_c_room;
               ret.push_back( new_gs );
               printGameState( new_gs );
            }
         }

      }

      // generate all the cases for the rooms
      if ( !empty_chars( curr.d_room ) and curr.d_room.second != 'D' )    // case for room a
      {
         pair<char, char> new_d_room;
         char first_out_d;
         if ( curr.d_room.first == '.' )
         {
            new_d_room  = make_pair( '.', '.' );
            first_out_d = curr.d_room.second;
         }
         else
         {
            new_d_room  = make_pair( '.', curr.d_room.second );
            first_out_d = curr.d_room.first;
         }

         int l = 2;                      // we want a range
         int r = 2;                      // two pointer approach to get the span
         while ( curr.hallway[r] == '.' )
         {
            ++r;
         }
         while ( curr.hallway[l] == '.' )
         {
            --l;
         }
         ++l;
         for ( int i = l; i < r; ++i )
         {
            if ( i != 2 and i != 4 and i != 6 and i != 8 )
            {
               game_state new_gs = curr;
               new_gs.hallway[i] = first_out_d;
               new_gs.d_room     = new_d_room;
               ret.push_back( new_gs );
               printGameState( new_gs );
            }
         }

      }
      // generate all the cases for the things in the hallway (i think they can only go into the things)
      for ( int i = 0; i < 11; ++i )
      {
         auto curr_c = curr.hallway[i];

         if ( curr_c != '.' )
         {
            if ( curr_c == 'A' and ( empty_chars( curr.a_room ) or curr.a_room.second == 'A' ) )    // our character is A and we have a potential run to a_room
            {
               int  goal       = 2;                                                                 // need unimpeded path to the A room
               int  i_pt       = i;
               bool do_nothing = false;                                                             // want to begin checking on the next one
               if ( goal > i_pt )
               {
                  ++i_pt;
                  while ( i_pt < goal )
                  {
                     if ( curr.hallway[i_pt] != '.' )    // something blocked us
                     {
                        do_nothing = true;
                        break;
                     }
                     ++i_pt;
                  }
               }
               else
               {
                  --i_pt;
                  while ( i_pt > goal )
                  {
                     if ( curr.hallway[i_pt] != '.' )
                     {
                        do_nothing = true;
                        break;
                     }
                     --i_pt;
                  }
               }
               if ( !do_nothing )    // insert a into a_room HURRAY
               {
                  game_state new_gs = curr;
                  new_gs.hallway[i] = '.';
                  if ( empty_chars( curr.a_room ) )
                  {
                     new_gs.a_room.second = 'A';
                  }
                  else
                  {
                     new_gs.a_room.first = 'A';
                  }
                  ret.push_back( new_gs );
                  //printGameState( new_gs );

               }
            }

            if ( curr_c == 'B' and ( empty_chars( curr.b_room ) or curr.b_room.second == 'B' ) )    // our character is A and we have a potential run to a_room
            {
               int  goal       = 4;                                                                 // need unimpeded path to the A room
               int  i_pt       = i;
               bool do_nothing = false;                                                             // want to begin checking on the next one
               if ( goal > i_pt )
               {
                  ++i_pt;
                  while ( i_pt < goal )
                  {
                     if ( curr.hallway[i_pt] != '.' )    // something blocked us
                     {
                        do_nothing = true;
                        break;
                     }
                     ++i_pt;
                  }
               }
               else
               {
                  --i_pt;
                  while ( i_pt > goal )
                  {
                     if ( curr.hallway[i_pt] != '.' )
                     {
                        do_nothing = true;
                        break;
                     }
                     --i_pt;
                  }
               }
               if ( !do_nothing )    // insert b into b_room HURRAY
               {
                  game_state new_gs = curr;
                  new_gs.hallway[i] = '.';
                  if ( empty_chars( curr.b_room ) )
                  {
                     new_gs.b_room.second = 'B';
                  }
                  else
                  {
                     new_gs.b_room.first = 'B';
                  }
                  ret.push_back( new_gs );
                  //printGameState( new_gs );

               }
            }
            if ( curr_c == 'C' and ( empty_chars( curr.c_room ) or curr.c_room.second == 'C' ) )    // our character is A and we have a potential run to a_room
            {
               int  goal       = 6;                                                                 // need unimpeded path to the A room
               int  i_pt       = i;
               bool do_nothing = false;                                                             // want to begin checking on the next one
               if ( goal > i_pt )
               {
                  ++i_pt;
                  while ( i_pt < goal )
                  {
                     if ( curr.hallway[i_pt] != '.' )    // something blocked us
                     {
                        do_nothing = true;
                        break;
                     }
                     ++i_pt;
                  }
               }
               else
               {
                  --i_pt;
                  while ( i_pt > goal )
                  {
                     if ( curr.hallway[i_pt] != '.' )
                     {
                        do_nothing = true;
                        break;
                     }
                     --i_pt;
                  }
               }
               if ( !do_nothing )    // insert b into b_room HURRAY
               {
                  game_state new_gs = curr;
                  new_gs.hallway[i] = '.';
                  if ( empty_chars( curr.c_room ) )
                  {
                     new_gs.c_room.second = 'C';
                  }
                  else
                  {
                     new_gs.c_room.first = 'C';
                  }
                  ret.push_back( new_gs );
                  //printGameState( new_gs );

               }
            }
            if ( curr_c == 'D' and ( empty_chars( curr.d_room ) or curr.d_room.second == 'D' ) )    // our character is A and we have a potential run to a_room
            {
               int  goal       = 8;                                                                 // need unimpeded path to the A room
               int  i_pt       = i;
               bool do_nothing = false;                                                             // want to begin checking on the next one
               if ( goal > i_pt )
               {
                  ++i_pt;
                  while ( i_pt < goal )
                  {
                     if ( curr.hallway[i_pt] != '.' )    // something blocked us
                     {
                        do_nothing = true;
                        break;
                     }
                     ++i_pt;
                  }
               }
               else
               {
                  --i_pt;
                  while ( i_pt > goal )
                  {
                     if ( curr.hallway[i_pt] != '.' )
                     {
                        do_nothing = true;
                        break;
                     }
                     --i_pt;
                  }
               }
               if ( !do_nothing )    // insert b into b_room HURRAY
               {
                  game_state new_gs = curr;
                  new_gs.hallway[i] = '.';
                  if ( empty_chars( curr.d_room ) )
                  {
                     new_gs.d_room.second = 'D';
                  }
                  else
                  {
                     new_gs.d_room.first = 'D';
                  }
                  ret.push_back( new_gs );
                  //printGameState( new_gs );

               }
            }
         }

      }

   }
   return ret;
}

int getEnergyDelta( game_state &curr, game_state &neighbor )
{
   int  hw_i = 0;
   char hw_was;
   char hw_now;
   if ( curr.hallway != neighbor.hallway )
   {
      for ( int i = 0; i < curr.hallway.size(); ++i )
      {
         if ( curr.hallway[i] != neighbor.hallway[i] )
         {
            hw_i == i;
            hw_was = curr.hallway[i];
            hw_now = neighbor.hallway[i];
         }
      }
   }

   if ( hw_now != '.' )
   {
   }
   else // we moved into
   {
      return get_move_into_energy()
   }
}

void partOne()
{
   auto goal = getGoal();
   assert( INIT != goal );

   priority_queue<game_state, vector<game_state>, Compare> discovered_pq;
   unordered_map<game_state, int, game_state_hasher> discovered_s;

   discovered_pq.push( INIT );
   discovered_s[INIT] = 0;

   while ( !discovered_pq.empty() )
   {
      auto current = discovered_pq.top();

      if ( current == goal )
      {
         cout << "Solution Found" << endl;
         printGameState( current );
         cout << endl;
      }
      discovered_pq.pop();
      discovered_s.erase( current );

      auto n_v = generate_neighbors( current );

      for ( auto &n_gs : n_v )
      {
         int temp_energy = current.energy + getEnergyDelta( current, n_gs );

         if ( discovered_s.find( n_gs ) == discovered_s.end() )
         {
            n_gs.energy        = temp_energy;
            discovered_s[n_gs] = temp_energy;
            discovered_pq.push( n_gs );
         }
         else
         {
            if ( discovered_s[n_gs] > temp_energy )
            {
               n_gs.energy        = temp_energy;
               discovered_s[n_gs] = temp_energy;
               discovered_pq.push( n_gs );
            }
         }
      }
   }
   cout << "FAILED" << endl;
}

int main( int argc, char * *argv )
{
   string filename = string( argv[1] );
   parseInput( filename );
   partOne();
}

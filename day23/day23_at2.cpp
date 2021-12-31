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

enum Space_t
{
   OB, // out of bounds
   EMPTY,
   A_t,
   B_t,
   C_t,
   D_t
};

using state_row_t = vector<Space_t>;
using game_state_t = vector<state_row_t>;

Space_t toEnum(char c)
{
   switch (c)
   {
   case '#':
      return OB;

      break;

   case '.':
      return EMPTY;

      break;

   case 'A':
      return A_t;

      break;

   case 'B':
      return B_t;

      break;

   case 'C':
      return C_t;

      break;

   case 'D':
      return D_t;

      break;

   default:
      break;
   }
}

char toChar(Space_t sp)
{
   switch (sp)
   {
   case OB:
      return '#';

      break;

   case EMPTY:
      return '.';

      break;

   case A_t:
      return 'A';

      break;

   case B_t:
      return 'B';

      break;

   case C_t:
      return 'C';

      break;

   case D_t:
      return 'D';

      break;

   default:
      break;
   }
}

void printGameState(game_state_t &gs)
{
   for (auto &row : gs)
   {
      for (auto &sp : row)
      {
         cout << toChar(sp);
      }
      cout << endl;
   }
   cout << endl;
}

vector<int> flattenGameState(const game_state_t &gs)
{
   vector<int> ret;
   for (auto &row : gs)
   {
      for (auto &sp : row)
      {
         ret.push_back(static_cast<int>(sp));
      }
   }
   return ret;
}

struct game_state_hasher
{
   std::size_t operator()(game_state_t const &gs) const
   {
      auto flattened = flattenGameState(gs);
      std::size_t seed = flattened.size();
      for (auto &i : flattened)
      {
         size_t s = static_cast<size_t>(i);
         seed ^= s + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
   }
};
struct hash_pair
{
   template <class T1, class T2>
   size_t operator()(const pair<T1, T2> &p) const
   {
      auto hash1 = hash<T1>{}(p.first);
      auto hash2 = hash<T2>{}(p.second);
      return hash1 ^ hash2;
   }
};

using gs_pq_t = pair<game_state_t, int>; // pairs a game_state to an energy value

class Compare
{
public:
   bool operator()(gs_pq_t &lhs, gs_pq_t &rhs)
   {
      return (lhs.second > rhs.second);
   }
};

game_state_t init;

void parseInput(string &fn)
{
   ifstream f;
   f.open(fn);
   string temp;
   while (getline(f, temp))
   {
      state_row_t temp_v;
      for (auto &c : temp)
      {
         temp_v.push_back(toEnum(c));
      }
      init.push_back(temp_v);
   }
   cout << "Initial Game State" << endl;
   printGameState(init);
}

pair<int, int> A_ROOM_1 = {2, 3};
pair<int, int> A_ROOM_2 = {3, 3};
pair<int, int> A_ROOM_3 = {4, 3};
pair<int, int> A_ROOM_4 = {5, 3};

pair<int, int> B_ROOM_1 = {2, 5};
pair<int, int> B_ROOM_2 = {3, 5}; // small example for now
pair<int, int> B_ROOM_3 = {4, 5};
pair<int, int> B_ROOM_4 = {5, 5};

pair<int, int> C_ROOM_1 = {2, 7};
pair<int, int> C_ROOM_2 = {3, 7}; // small example for now
pair<int, int> C_ROOM_3 = {4, 7};
pair<int, int> C_ROOM_4 = {5, 7};

pair<int, int> D_ROOM_1 = {2, 9};
pair<int, int> D_ROOM_2 = {3, 9}; // small example for now
pair<int, int> D_ROOM_3 = {4, 9};
pair<int, int> D_ROOM_4 = {5, 9};

pair<int, int> ROOM_A_ENTR = {1, 3};
pair<int, int> ROOM_B_ENTR = {1, 5}; // small example for now

vector<pair<int, int>> genNeighbors(pair<int, int> &curr)
{
   auto f = make_pair(curr.first - 1, curr.second);
   auto s = make_pair(curr.first + 1, curr.second);
   auto t = make_pair(curr.first, curr.second - 1);
   auto fr = make_pair(curr.first, curr.second + 1);
   vector<pair<int, int>> ret;
   ret.push_back(f);
   ret.push_back(s);
   ret.push_back(t);
   ret.push_back(fr);
   return ret;
}

bool isValidNeighbor(pair<int, int> neighbor, const game_state_t &gs)
{
   return gs[neighbor.first][neighbor.second] == EMPTY;
}

pair<bool, int> quickDfs(pair<int, int> &start, pair<int, int> &goal, const game_state_t &gs)
{
   stack<pair<pair<int, int>, int>> s; // [(r,c),d];
   unordered_set<pair<int, int>, hash_pair> visited;
   s.push(make_pair(start, 0));
   visited.insert(start);

   //cout << "made it here?" << endl;

   while (!s.empty())
   {
      auto curr = s.top();
      s.pop();
      //visited.erase( curr.first );
      if (curr.first == goal)
      {
         return make_pair(true, curr.second);
      }

      for (auto &nb : genNeighbors(curr.first))
      {
         //cout << nb.first << " " << nb.second << endl;
         if (visited.find(nb) == visited.end() and isValidNeighbor(nb, gs)) // if we havent already been here
         {
            s.push(make_pair(nb, curr.second + 1));
            visited.insert(nb);
         }
      }
   }

   return make_pair(false, -1);
}

Space_t getSpotType(pair<int, int> &sp, const game_state_t &gs)
{
   return gs[sp.first][sp.second];
}

struct Move
{
   Move(pair<int, int> start_, pair<int, int> end_, int steps_) : start(start_), end(end_), steps(steps_) {}
   pair<int, int> start;
   pair<int, int> end;
   int steps;
};

bool checkHallway(vector<Move> &ret, const game_state_t &gs) // will return true if we can skip the next step
{
   for (int col = 0; col < gs[1].size(); ++col) // row one, checking for guys in the hallway
   {
      auto start_space = make_pair(1, col);
      switch (gs[1][col])
      {
      case A_t:
         // if all spots empty, run dfs to spot_4
         if (getSpotType(A_ROOM_1, gs) == EMPTY and getSpotType(A_ROOM_2, gs) == EMPTY and getSpotType(A_ROOM_3, gs) == EMPTY and getSpotType(A_ROOM_4, gs) == EMPTY)
         {
            //cout << "Made it Here" << endl;

            auto check = quickDfs(start_space, A_ROOM_4, gs);
            if (check.first)
            {
               //cout << "Move to A home in " << check.second << " steps" << endl;
               Move ins(start_space, A_ROOM_4, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue; // no legal moves for this space
            }
         }
         else if (getSpotType(A_ROOM_2, gs) == A_t and getSpotType(A_ROOM_3, gs) == A_t and getSpotType(A_ROOM_4, gs) == A_t and getSpotType(A_ROOM_1, gs) == EMPTY)
         { // else if spot 2,3,4 has an A, run dfs to spot_1
            auto check = quickDfs(start_space, A_ROOM_1, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, A_ROOM_1, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else if (getSpotType(A_ROOM_3, gs) == A_t and getSpotType(A_ROOM_4, gs) == A_t and getSpotType(A_ROOM_2, gs) == EMPTY and getSpotType(A_ROOM_1, gs) == EMPTY)
         {
            // run dfs to spot 2
            auto check = quickDfs(start_space, A_ROOM_2, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, A_ROOM_2, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else if (getSpotType(A_ROOM_3, gs) == EMPTY and getSpotType(A_ROOM_4, gs) == A_t and getSpotType(A_ROOM_2, gs) == EMPTY and getSpotType(A_ROOM_1, gs) == EMPTY)
         { // dfs to spot 3( 4 is only one filled)
            auto check = quickDfs(start_space, A_ROOM_3, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, A_ROOM_3, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else
         {
            /* DO NOTHING THIS PIECE IS STUCK FOR NOW*/
         }

         break;

      case B_t:
         // if both spots empty, run dfs to spot_2
         if (getSpotType(B_ROOM_1, gs) == EMPTY and getSpotType(B_ROOM_2, gs) == EMPTY and getSpotType(B_ROOM_3, gs) == EMPTY and getSpotType(B_ROOM_4, gs) == EMPTY)
         {
            //cout << "Made it Here" << endl;

            auto check = quickDfs(start_space, B_ROOM_4, gs);
            if (check.first)
            {
               //cout << "Move to A home in " << check.second << " steps" << endl;
               Move ins(start_space, B_ROOM_4, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue; // no legal moves for this space
            }
         }
         else if (getSpotType(B_ROOM_2, gs) == B_t and getSpotType(B_ROOM_3, gs) == B_t and getSpotType(B_ROOM_4, gs) == B_t and getSpotType(B_ROOM_1, gs) == EMPTY)
         { // else if spot 2,3,4 has an A, run dfs to spot_1
            auto check = quickDfs(start_space, B_ROOM_1, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, B_ROOM_1, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else if (getSpotType(B_ROOM_3, gs) == B_t and getSpotType(B_ROOM_4, gs) == B_t and getSpotType(B_ROOM_2, gs) == EMPTY and getSpotType(B_ROOM_1, gs) == EMPTY)
         {
            // run dfs to spot 2
            auto check = quickDfs(start_space, B_ROOM_2, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, B_ROOM_2, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else if (getSpotType(B_ROOM_3, gs) == EMPTY and getSpotType(B_ROOM_4, gs) == B_t and getSpotType(B_ROOM_2, gs) == EMPTY and getSpotType(B_ROOM_1, gs) == EMPTY)
         { // dfs to spot 3( 4 is only one filled)
            auto check = quickDfs(start_space, B_ROOM_3, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, B_ROOM_3, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else
         {
            /* DO NOTHING THIS PIECE IS STUCK FOR NOW*/
         }

         break;

      case C_t:
         // if both spots empty, run dfs to spot_2
         if (getSpotType(C_ROOM_1, gs) == EMPTY and getSpotType(C_ROOM_2, gs) == EMPTY and getSpotType(C_ROOM_3, gs) == EMPTY and getSpotType(C_ROOM_4, gs) == EMPTY)
         {
            //cout << "Made it Here" << endl;

            auto check = quickDfs(start_space, C_ROOM_4, gs);
            if (check.first)
            {
               //cout << "Move to A home in " << check.second << " steps" << endl;
               Move ins(start_space, C_ROOM_4, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue; // no legal moves for this space
            }
         }
         else if (getSpotType(C_ROOM_2, gs) == C_t and getSpotType(C_ROOM_3, gs) == C_t and getSpotType(C_ROOM_4, gs) == C_t and getSpotType(C_ROOM_1, gs) == EMPTY)
         { // else if spot 2,3,4 has an A, run dfs to spot_1
            auto check = quickDfs(start_space, C_ROOM_1, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, C_ROOM_1, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else if (getSpotType(C_ROOM_3, gs) == C_t and getSpotType(C_ROOM_4, gs) == C_t and getSpotType(C_ROOM_2, gs) == EMPTY and getSpotType(C_ROOM_1, gs) == EMPTY)
         {
            // run dfs to spot 2
            auto check = quickDfs(start_space, C_ROOM_2, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, C_ROOM_2, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else if (getSpotType(C_ROOM_3, gs) == EMPTY and getSpotType(C_ROOM_4, gs) == C_t and getSpotType(C_ROOM_2, gs) == EMPTY and getSpotType(C_ROOM_1, gs) == EMPTY)
         { // dfs to spot 3( 4 is only one filled)
            auto check = quickDfs(start_space, C_ROOM_3, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, C_ROOM_3, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else
         {
            /* DO NOTHING THIS PIECE IS STUCK FOR NOW*/
         }

         break;

      case D_t:
         // if both spots empty, run dfs to spot_2
         if (getSpotType(D_ROOM_1, gs) == EMPTY and getSpotType(D_ROOM_2, gs) == EMPTY and getSpotType(D_ROOM_3, gs) == EMPTY and getSpotType(D_ROOM_4, gs) == EMPTY)
         {
            //cout << "Made it Here" << endl;

            auto check = quickDfs(start_space, D_ROOM_4, gs);
            if (check.first)
            {
               //cout << "Move to A home in " << check.second << " steps" << endl;
               Move ins(start_space, D_ROOM_4, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue; // no legal moves for this space
            }
         }
         else if (getSpotType(D_ROOM_2, gs) == D_t and getSpotType(D_ROOM_3, gs) == D_t and getSpotType(D_ROOM_4, gs) == D_t and getSpotType(D_ROOM_1, gs) == EMPTY)
         { // else if spot 2,3,4 has an A, run dfs to spot_1
            auto check = quickDfs(start_space, D_ROOM_1, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, D_ROOM_1, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else if (getSpotType(D_ROOM_3, gs) == D_t and getSpotType(D_ROOM_4, gs) == D_t and getSpotType(D_ROOM_2, gs) == EMPTY and getSpotType(D_ROOM_1, gs) == EMPTY)
         {
            // run dfs to spot 2
            auto check = quickDfs(start_space, D_ROOM_2, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, D_ROOM_2, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else if (getSpotType(D_ROOM_3, gs) == EMPTY and getSpotType(D_ROOM_4, gs) == D_t and getSpotType(D_ROOM_2, gs) == EMPTY and getSpotType(D_ROOM_1, gs) == EMPTY)
         { // dfs to spot 3( 4 is only one filled)
            auto check = quickDfs(start_space, D_ROOM_3, gs);
            if (check.first)
            {
               //cout << "Move to A home into the first slot in " << check.second << " steps" << endl;
               Move ins(start_space, D_ROOM_3, check.second);
               ret.push_back(ins);
               return true;
            }
            else
            {
               continue;
            }
         }
         else
         {
            /* DO NOTHING THIS PIECE IS STUCK FOR NOW*/
         }

         break;

      default:
         break;
      }
   }
   return false;
}

void checkRest(vector<Move> &ret, const game_state_t &gs)
{
   for (int r = 2; r < gs.size() - 1; ++r)
   {
      for (int c = 0; c < gs[r].size(); ++c)
      {
         if (gs[r][c] != EMPTY and gs[r][c] != OB and gs[r - 1][c] == EMPTY) // must have empty space above otherwise dont bother
         {
            auto start_spot = make_pair(r, c);

            switch (gs[r][c])
            {
            case A_t:
               if (start_spot == A_ROOM_1 and getSpotType(A_ROOM_2, gs) == A_t and getSpotType(A_ROOM_3, gs) == A_t and getSpotType(A_ROOM_4, gs) == A_t) // its filled
               {
                  //cout << "A room Filled" << endl;
                  continue;
               }
               if (start_spot == A_ROOM_2 and getSpotType(A_ROOM_3, gs) == A_t and getSpotType(A_ROOM_4, gs) == A_t) // A's below it all the way down
               {
                  //cout << "A in spot 2 of A room will stay put" << endl;
                  continue;
               }

               if (start_spot == A_ROOM_3 and getSpotType(A_ROOM_4, gs) == A_t)
               {
                  continue;
               }
               if (start_spot == A_ROOM_4)
               {
                  continue;
               }

               if (start_spot == A_ROOM_1 and getSpotType(A_ROOM_2, gs) != A_t)
               {
                  //cout << "A in spot 1 of room 1 is blocking another spot type" << endl;
                  // generate the moves out
               }
               else
               {
                  //cout << "A is not in one of its home spots so this is not a special case" << endl;
                  // generate the moves out
               }

               break;

            case B_t:
               if (start_spot == B_ROOM_1 and getSpotType(B_ROOM_2, gs) == B_t and getSpotType(B_ROOM_3, gs) == B_t and getSpotType(B_ROOM_4, gs) == B_t) // its filled
               {
                  //cout << "A room Filled" << endl;
                  continue;
               }
               if (start_spot == B_ROOM_2 and getSpotType(B_ROOM_3, gs) == B_t and getSpotType(B_ROOM_4, gs) == B_t) // A's below it all the way down
               {
                  //cout << "A in spot 2 of A room will stay put" << endl;
                  continue;
               }

               if (start_spot == B_ROOM_3 and getSpotType(B_ROOM_4, gs) == B_t)
               {
                  continue;
               }
               if (start_spot == B_ROOM_4)
               {
                  continue;
               }
               if (start_spot == B_ROOM_1 and getSpotType(B_ROOM_2, gs) != B_t)
               {
                  //cout << "B in spot 1 of room 1 is blocking another spot type" << endl;
                  // generate the moves out
               }
               else
               {
                  //cout << "B is not in one of its home spots so this is not a special case" << endl;
                  // generate the moves out
               }

               break;

            case C_t:
               if (start_spot == C_ROOM_1 and getSpotType(C_ROOM_2, gs) == C_t and getSpotType(C_ROOM_3, gs) == C_t and getSpotType(C_ROOM_4, gs) == C_t) // its filled
               {
                  //cout << "A room Filled" << endl;
                  continue;
               }
               if (start_spot == C_ROOM_2 and getSpotType(C_ROOM_3, gs) == C_t and getSpotType(C_ROOM_4, gs) == C_t) // A's below it all the way down
               {
                  //cout << "A in spot 2 of A room will stay put" << endl;
                  continue;
               }

               if (start_spot == C_ROOM_3 and getSpotType(C_ROOM_4, gs) == C_t)
               {
                  continue;
               }
               if (start_spot == C_ROOM_4)
               {
                  continue;
               }

               if (start_spot == C_ROOM_1 and getSpotType(C_ROOM_2, gs) != C_t)
               {
                  //cout << "C in spot 1 of room 1 is blocking another spot type" << endl;
                  // generate the moves out
               }
               else
               {
                  //cout << "C is not in one of its home spots so this is not a special case" << endl;
                  // generate the moves out
               }

               break;

            case D_t:
               if (start_spot == D_ROOM_1 and getSpotType(D_ROOM_2, gs) == D_t and getSpotType(D_ROOM_3, gs) == D_t and getSpotType(D_ROOM_4, gs) == D_t) // its filled
               {
                  //cout << "A room Filled" << endl;
                  continue;
               }
               if (start_spot == D_ROOM_2 and getSpotType(D_ROOM_3, gs) == D_t and getSpotType(D_ROOM_4, gs) == D_t) // A's below it all the way down
               {
                  //cout << "A in spot 2 of A room will stay put" << endl;
                  continue;
               }

               if (start_spot == D_ROOM_3 and getSpotType(D_ROOM_4, gs) == D_t)
               {
                  continue;
               }
               if (start_spot == D_ROOM_4)
               {
                  continue;
               }

               if (start_spot == D_ROOM_1 and getSpotType(D_ROOM_2, gs) != D_t)
               {
                  //cout << "D in spot 1 of room 1 is blocking another spot type" << endl;
                  // generate the moves out
               }
               else
               {
                  //cout << "D is not in one of its home spots so this is not a special case" << endl;
                  // generate the moves out
               }

               break;

            default:
               break;
            }

            for (int c = 1; c < gs[1].size() - 1; ++c) // attempt to get to every point in the hallway
            {
               if (c != 3 and c != 5 and c != 7 and c != 9) // dont even think about moving in front of a room
               {
                  auto end_space = make_pair(1, c);

                  auto check = quickDfs(start_spot, end_space, gs);
                  if (check.first)
                  {
                     //cout << "Making move out of room to hallway" << endl;
                     Move ins(start_spot, end_space, check.second);
                     ret.push_back(ins);
                  }
               }
            }

            // first part does not matter what type of thing it is. it will simply generate all the places in the hallway that it can go

            // second part will be a switch that will check the direct to home things like before
         }
      }
   }
}

int getMultiplier(Space_t t)
{
   switch (t)
   {
   case A_t:
      return 1;

      break;
   case B_t:
      return 10;

      break;
   case C_t:
      return 100;

      break;
   case D_t:
      return 1000;

      break;

   default:
      return 1;

      break;
   }
}

unordered_map<game_state_t, vector<Move>, game_state_hasher> move_chache;

vector<Move> getPotentialMoveSet(const game_state_t &gs) // tuple. row_init, col_init, row_end, col_end, dist
{
   vector<Move> ret;
   bool skipNext = checkHallway(ret, gs);
   if (!skipNext)
   {
      checkRest(ret, gs);
   }
   return ret;
}

unordered_map<game_state_t, vector<pair<game_state_t, int>>, game_state_hasher> gs_chache;

vector<pair<game_state_t, int>> genPotentialGameStates(vector<Move> moves, const game_state_t &gs)
{
   vector<pair<game_state_t, int>> ret;

   for (auto &mv : moves)
   {
      auto new_gs = gs;

      auto temp = getSpotType(mv.start, new_gs);
      auto temp2 = getSpotType(mv.end, new_gs);
      new_gs[mv.start.first][mv.start.second] = temp2;
      new_gs[mv.end.first][mv.end.second] = temp;
      int delta = mv.steps * getMultiplier(temp2) * getMultiplier(temp);
      //cout << "New game state with delta: " << delta << endl;
      //printGameState(new_gs);

      ret.emplace_back(make_pair(new_gs, delta));
   }
   return ret;
}

game_state_t getGoal()
{
   game_state_t goal = init;
   goal[A_ROOM_1.first][A_ROOM_1.second] = A_t;
   goal[A_ROOM_2.first][A_ROOM_2.second] = A_t;
   goal[A_ROOM_3.first][A_ROOM_3.second] = A_t;
   goal[A_ROOM_4.first][A_ROOM_4.second] = A_t;

   goal[B_ROOM_1.first][B_ROOM_1.second] = B_t;
   goal[B_ROOM_2.first][B_ROOM_2.second] = B_t;
   goal[B_ROOM_3.first][B_ROOM_3.second] = B_t;
   goal[B_ROOM_4.first][B_ROOM_4.second] = B_t;

   goal[C_ROOM_1.first][C_ROOM_1.second] = C_t;
   goal[C_ROOM_2.first][C_ROOM_2.second] = C_t;
   goal[C_ROOM_3.first][C_ROOM_3.second] = C_t;
   goal[C_ROOM_4.first][C_ROOM_4.second] = C_t;

   goal[D_ROOM_1.first][D_ROOM_1.second] = D_t;
   goal[D_ROOM_2.first][D_ROOM_2.second] = D_t;
   goal[D_ROOM_3.first][D_ROOM_3.second] = D_t;
   goal[D_ROOM_4.first][D_ROOM_4.second] = D_t;

   return goal;
}

void partOne()
{
   auto goal = getGoal();

   cout << "Goal" << endl;
   printGameState(goal);

   priority_queue<gs_pq_t, vector<gs_pq_t>, Compare> discovered_pq;    // pair of gamestate and associated energy
   unordered_map<game_state_t, int, game_state_hasher> discovered_map; // int is the energy

   auto start = make_pair(init, 0);
   discovered_pq.push(start);
   discovered_map[start.first] = start.second;

   while (!discovered_pq.empty())
   {
      auto curr = discovered_pq.top();

      if (curr.first == goal)
      {
         cout << "solution found" << endl;
         printGameState(curr.first);
         cout << endl;
         cout << curr.second << endl;
         return;
      }
      discovered_pq.pop();
      discovered_map.erase(curr.first);

      auto neighbor_gs = genPotentialGameStates(getPotentialMoveSet(curr.first), curr.first);

      for (auto &neighbor : neighbor_gs)
      {
         int temp_energy = curr.second + neighbor.second;

         if (discovered_map.find(neighbor.first) == discovered_map.end())
         {
            neighbor.second = temp_energy;
            discovered_map[neighbor.first] = temp_energy;
            discovered_pq.push(neighbor);
         }
         else
         {
            if (discovered_map[neighbor.first] > temp_energy)
            {
               // if temp energy is lower, update it
               neighbor.second = temp_energy;
               discovered_map[neighbor.first] = temp_energy;
               discovered_pq.push(neighbor);
            }
         }
      }
   }
   cout << "Failed" << endl;

   //auto potential_moves = getPotentialMoveSet( init );

   //auto potential_game_states = genPotentialGameStates( potential_moves, init );
}

int main(int argc, char **argv)
{
   string filename = string(argv[1]);
   parseInput(filename);
   partOne();
}

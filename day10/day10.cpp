#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

vector<string> chunks;
vector<string> incomplete;

void parseInput()
{
   ifstream f;
   f.open( "day10.txt" );

   string temp;
   while ( getline( f, temp ) )
   {
      chunks.push_back( temp );
   }
}

void partOne()
{
   int score_sum = 0;
   for ( auto &chunk : chunks )
   {
      int chunk_score = 0;
      stack<char> c_s;
      bool good = true;
      for ( auto &c : chunk )
      {
         //cout << c << " ";
         if ( c == '(' or c == '[' or c == '{' or c == '<' ) { c_s.push( c ); }
         else
         {
            char t = c_s.top();
            if ( c == ')' and t != '(' )
            {
               //cout << "got a ) when expecting something else";
               chunk_score = 3;
               good        = false;
               break;
            }
            else if ( c == ']' and t != '[' )
            {
               // cout << "got a ] when expecting something else";
               chunk_score = 57;
               good        = false;
               break;
            }
            else if ( c == '}' and t != '{' )
            {
               // cout << "got a } when expecting something else";
               chunk_score = 1197;
               good        = false;
               break;
            }
            else if ( c == '>' and t != '<' )
            {
               //  cout << "got a > when expecting something else";
               chunk_score = 25137;
               good        = false;
               break;
            }
            else
            {
               // all good
               c_s.pop();
            }
         }
      }
      if ( good )
      {
         incomplete.push_back( chunk );
      }
      // cout << endl;
      score_sum += chunk_score;
   }
   cout << score_sum << endl;

}

void partTwo()
{
   vector<string> autos;
   for ( auto &chunk : incomplete )
   {
      stack<char> c_s;
      bool good = true;
      for ( auto &c : chunk )
      {
         //cout << c << " ";
         if ( c == '(' or c == '[' or c == '{' or c == '<' ) { c_s.push( c ); }
         else
         {
            c_s.pop();
         }
      }
      string auto_complete = "";
      while ( !c_s.empty() )
      {
         char t = c_s.top();
         if ( t == '(' )
         {
            auto_complete += ')';
         }
         else if ( t == '<' )
         {
            auto_complete += '>';
         }
         else if ( t == '[' )
         {
            auto_complete += ']';
         }
         else if ( t == '{' )
         {
            auto_complete += '}';
         }
         c_s.pop();
      }
      autos.push_back( auto_complete );

   }
   vector<long long int> scores;
   for ( auto &str : autos )
   {
      long long int score = 0;
      for ( auto &c : str )
      {
         score *= 5;
         if ( c == ')' ) { score += 1; }
         if ( c == ']' ) { score += 2; }
         if ( c == '}' ) { score += 3; }
         if ( c == '>' ) { score += 4; }

      }
      scores.push_back( score );
   }

   auto mid = begin( scores );
   advance( mid, scores.size() / 2 );
   nth_element( begin( scores ), mid, end( scores ) );

   cout << *mid;

}

int main()
{
   parseInput();
   partOne();
   partTwo();
}

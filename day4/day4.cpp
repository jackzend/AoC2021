#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>

using namespace std;

struct gameBoard // represents a bingo game board
{
   vector<vector<pair<int, bool> > > data;
   bool                              isWinner = false;
};

vector<int> nums;      // the numbers called

vector<gameBoard> gbs; // array of bingo game boards

void parse_input()
{
   ifstream f;
   f.open( "day4.txt" );
   string s;
   getline( f, s );
   //cout << s << endl;
   string delimiter = ",";
   size_t pos       = 0;
   std::string token;
   while ( ( pos = s.find( delimiter ) ) != std::string::npos )
   {
      token = s.substr( 0, pos );
      nums.push_back( stoi( token ) );
      //std::cout << token << std::endl;
      s.erase( 0, pos + delimiter.length() );
   }
   //std::cout << s << std::endl;
   nums.push_back( stoi( s ) );
   gameBoard temp_gb;
   getline( f, s );
   while ( getline( f, s ) )
   {
      if ( s.empty() )
      {
         gbs.push_back( temp_gb );
         for ( auto &row: temp_gb.data )
         {
            row.clear();
         }
         temp_gb.data.clear();
      }
      else
      {
         stringstream sstr( s );
         vector<pair<int, bool> > temp_v;
         int t;
         while ( sstr >> t )
         {
            temp_v.emplace_back( make_pair( t, false ) );
         }
         temp_gb.data.push_back( temp_v );
      }
   }

   for ( auto &gb : gbs )
   {
      cout << "A GAMEBOARD" << endl;
      for ( auto &row : gb.data )
      {
         for ( auto &i : row )
         {
            cout << i.first << " ";
         }
         cout << endl;
      }
      cout << endl;
   }
   f.close();
}

bool row_is_bingo( gameBoard &gb )
{
   for ( auto &row : gb.data )
   {
      bool flag = true;
      for ( auto &item : row )
      {
         if ( item.second == false )
         {
            flag = false;
            break;
         }
      }
      if ( flag )
      {
         cout << "row is bingo" << endl;
         return true;
      }
   }
   return false;
}

bool col_is_bingo( gameBoard &gb )
{
   for ( size_t i = 0; i < gb.data.size(); ++i )
   {
      bool flag = true;
      for ( size_t j = 0; j < gb.data.size(); ++j )
      {
         if ( gb.data[j][i].second == false )
         {
            flag = false;
            break;
         }
      }
      if ( flag )
      {
         return true;
      }
   }
   return false;
}

bool diags_is_bingo( gameBoard &gb )
{
   bool first_flag = true;
   for ( size_t i = 0; i < gb.data.size(); ++i )
   {
      if ( gb.data[i][i].second == false )
      {
         first_flag = false;
         break;
      }
   }
   if ( first_flag )
   {
      return true;
   }

   bool second_flag = true;
   for ( size_t i = 0; i < gb.data.size(); ++i )
   {
      if ( gb.data[i][gb.data.size() - i].second == false )
      {
         second_flag = false;
         break;
      }
   }
   if ( second_flag )
   {
      return true;
   }
   return false;

}

bool winning_gb( gameBoard &gb )
{
   return ( row_is_bingo( gb ) or col_is_bingo( gb ) or diags_is_bingo( gb ) );

}

int calculate_score( gameBoard &gb )
{
   int sum = 0;
   for ( auto &row : gb.data )
   {
      for ( auto &item : row )
      {
         if ( item.second == false )
         {
            sum += item.first;
         }
      }
   }
   return sum;
}

int main()
{
   parse_input();
   for ( auto &num : nums )
   {
      for ( auto &gb : gbs )
      {
         if ( !gb.isWinner )
         {
            for ( auto &row : gb.data )
            {
               for ( auto &item : row )
               {
                  if ( num == item.first )
                  {
                     item.second = true;
                  }
               }
            }
            if ( winning_gb( gb ) )
            {
               gb.isWinner = true;
               cout << calculate_score( gb ) << " winning number: " << num << endl;
            }
         }
      }

   }

}

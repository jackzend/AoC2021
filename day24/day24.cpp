#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
#include <vector>

using namespace std;

struct inp
{
   int divider;
   int checker;
   int adder;

};
vector<inp> inp_v;
void parseInput()
{
   fstream f;
   f.open( "day24.txt" );

   for ( int j = 0; j < 14; ++j )
   {
      inp    temp_inp;
      string temp;

      for ( int i = 0; i < 18; ++i )
      {
         getline( f, temp );
         if ( i == 4 )
         {
            stringstream ss( temp );
            string junk;
            ss >> junk >> junk;
            ss >> temp_inp.divider;
         }
         else if ( i == 5 )
         {
            stringstream ss( temp );
            string junk;
            ss >> junk >> junk;
            ss >> temp_inp.checker;
         }
         else if ( i == 15 )
         {
            stringstream ss( temp );
            string junk;
            ss >> junk >> junk;
            ss >> temp_inp.adder;
         }

      }
      inp_v.push_back( temp_inp );
      cout << "Div: " << temp_inp.divider << " Check: " << temp_inp.checker << " Adder: " << temp_inp.adder << endl;
   }
}

int main()
{
   parseInput();
   stack<int> s;

}

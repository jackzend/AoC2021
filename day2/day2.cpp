#include <iostream>
#include <fstream>

using namespace std;
int main()
{
   ifstream f;
   f.open( "day2.txt" );

   string direction;
   int    dist;

   int depth           = 0;
   int horizontal_dist = 0;
   int aim             = 0;

   while ( f >> direction >> dist )
   {
      if ( direction == "forward" )
      {
         horizontal_dist += dist;
         depth           += ( aim * dist );
      }
      else if ( direction == "down" )
      {
         aim += dist;
      }
      else if ( direction == "up" )
      {
         aim -= dist;
      }
   }

   cout << depth * horizontal_dist;
}

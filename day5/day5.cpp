#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct line
{
   int x1, x2, y1, y2;
};

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

unordered_map<pair<int, int>, int, hash_pair> coord_count;

vector<line> lines_v;

void parse_input()
{
   ifstream f;
   f.open( "day5.txt" );

   string temp;
   while ( getline( f, temp ) )
   {
      vector<int> v;
      string temp1 = "";
      for ( auto c : temp )
      {
         if ( isdigit( c ) )
         {
            temp1 += c;
         }
         else
         {
            //cout << temp1 << " ";
            try
            {
               int t = stoi( temp1 );
               v.push_back( t );
            }
            catch ( std::invalid_argument const &e )
            {
               //cout << e.what() << endl;
            }
            temp1.clear();
         }
      }
      if ( !temp1.empty() )
      {
         //cout << temp1 << endl;
         int t = stoi( temp1 );
         v.push_back( t );
         temp1.clear();
      }
      for ( auto i : v )
      {
         cout << i << " ";
      }
      cout << endl;

      line l;
      l.x1 = v[0];
      l.y1 = v[1];
      l.x2 = v[2];
      l.y2 = v[3];

      lines_v.push_back( l );
   }
}

bool isVertical( line &l )
{
   if ( l.x1 == l.x2 )
   {
      return true;
   }
   return false;
}
bool isHorizontal( line &l )
{
   if ( l.y1 == l.y2 )
   {
      return true;
   }
   return false;
}

bool is45degrees( line &l )
{
   if ( abs( l.y1 - l.y2 ) == abs( l.x1 - l.x2 ) )
   {
      return true;
   }
   return false;
}

vector<pair<int, int> > getCoords( line &l )
{
   vector<pair<int, int> > ret;
   if ( isVertical( l ) )
   {
      int begin, end;

      if ( l.y1 < l.y2 )
      {
         begin = l.y1;
         end   = l.y2;
      }
      else
      {
         begin = l.y2;
         end   = l.y1;
      }
      for ( int i = begin; i <= end; ++i )
      {
         ret.emplace_back( make_pair( l.x1, i ) ); // get intermitten points
      }
   }
   else if ( isHorizontal( l ) )
   {
      int begin, end;
      if ( l.x1 < l.x2 )
      {
         begin = l.x1;
         end   = l.x2;
      }
      else
      {
         begin = l.x2;
         end   = l.x1;
      }
      for ( int i = begin; i <= end; ++i )
      {
         ret.emplace_back( make_pair( i, l.y1 ) ); // get intermitten points
      }
   }
   else if ( is45degrees( l ) )
   {
      int begin_x, begin_y, end_x, end_y;
      if ( ( l.x1 < l.x2 ) and ( l.y1 < l.y2 ) )
      {
         begin_x = l.x1;
         end_x   = l.x2;
         begin_y = l.y1;
         end_y   = l.y2;

         for ( int i = begin_x, j = begin_y; i <= end_x; ++i, ++j )
         {
            ret.emplace_back( make_pair( i, j ) );
         }
      }
      else if ( ( l.x2 < l.x1 ) and ( l.y2 < l.y1 ) )
      {
         begin_x = l.x2;
         end_x   = l.x1;
         begin_y = l.y2;
         end_y   = l.y1;
         for ( int i = begin_x, j = begin_y; i <= end_x; ++i, ++j )
         {
            ret.emplace_back( make_pair( i, j ) );
         }
      }
      else if ( ( l.x1 < l.x2 ) and ( l.y1 > l.y2 ) )
      {
         begin_x = l.x1;
         end_x   = l.x2;
         begin_y = l.y1;
         end_y   = l.y2;

         for ( int i = begin_x, j = begin_y; i <= end_x; ++i, --j )
         {
            ret.emplace_back( make_pair( i, j ) );
         }
      }
      else if ( ( l.x2 < l.x1 ) and ( l.y2 > l.y1 ) )
      {
         begin_x = l.x2;
         end_x   = l.x1;
         begin_y = l.y2;
         end_y   = l.y1;
         for ( int i = begin_x, j = begin_y; i <= end_x; ++i, --j )
         {
            ret.emplace_back( make_pair( i, j ) );
         }
      }
   }
   cout << "processed line starting at: ";
   for ( auto &i: ret )
   {
      cout << "(" << i.first << "," << i.second << ") ";
   }
   cout << endl;

   return ret;
}

int part1()
{
   for ( auto &l : lines_v )
   {
      if ( isHorizontal( l ) or isVertical( l ) or is45degrees( l ) )
      {
         vector<pair<int, int> > temp = getCoords( l );
         for ( auto &coord : temp )
         {
            if ( coord_count.find( coord ) != coord_count.end() )
            {
               coord_count[coord] += 1;
            }
            else
            {
               coord_count[coord] = 1;
            }
         }
      }
   }

   int c = 0;
   for ( auto & [key, value] : coord_count )
   {
      if ( value > 1 )
      {
         ++c;
      }
   }
   return c;

}

int main()
{
   parse_input();
   cout << part1();

}

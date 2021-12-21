#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <array>
#include <eigen3/Eigen/Eigen>
#include <set>
#include <sstream>
#include <string>
#include <cassert>

using namespace std;

using namespace Eigen;

using point_3d = tuple<int, int, int>;
struct Scanner
{
   int                    scanner_id;
   vector<Vector3i>       point_vector;
   vector<set<point_3d> > rel_pos_sets; // will be a vector with 24 of such sets;
};

vector<Scanner> scan_v;

set<point_3d> all_overlaps;

template<typename T>
struct matrix_hash : std::unary_function<T, size_t>
{
   std::size_t operator()( T const &matrix ) const
   {
      // Note that it is oblivious to the storage order of Eigen matrix (column- or
      // row-major). It will give you the same hash value for two different matrices if they
      // are the transpose of each other in different storage order.
      size_t seed = 0;
      for ( size_t i = 0; i < matrix.size(); ++i )
      {
         auto elem = *( matrix.data() + i );
         seed ^= std::hash<typename T::Scalar>()( elem ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
      }
      return seed;
   }
};

IOFormat CleanFmt( 4, 0, ", ", "\n", "[", "]" );

Matrix3i genMatrix( double roll, double yaw, double pitch )
{
   AngleAxisd rollAngle( roll, Vector3d::UnitZ() );
   AngleAxisd yawAngle( yaw, Vector3d::UnitY() );
   AngleAxisd pitchAngle( pitch, Eigen::Vector3d::UnitX() );

   Quaternion<double> q = rollAngle * yawAngle * pitchAngle;

   return q.matrix().cast<int>();
}

unordered_set<Matrix3i, matrix_hash<Matrix3i> > matSet;

void generateRotMatrices()
{
   vector<double> deg{ 0, M_PI / 2, M_PI, 3 * ( M_PI / 2 ) };
   for ( int i = 0; i < 4; ++i )
   {
      for ( int j = 0; j < 4; ++j )
      {
         for ( int k = 0; k < 4; ++k )
         {
            auto mat = genMatrix( deg[k], deg[j], deg[i] );

            if ( mat.determinant() == 1 )
            {
               matSet.insert( mat );
            }
         }
      }
   }
   for ( auto &k : matSet )
   {
      cout << k.format( CleanFmt ) << endl << endl;
   }
   cout << "Num Mats: " << matSet.size() << endl;
}

void parseInput()
{
   ifstream f;
   f.open( "day19_test.txt" );

   string temp;
   int    scanner_num = 0;
   while ( getline( f, temp ) )
   {
      if ( temp[0] == '-' and temp[1] == '-' )
      {
         //cout << "Scanner number " << scanner_num << endl;

         Scanner t_s;
         t_s.scanner_id = scanner_num;
         ++scanner_num;
         while ( !temp.empty() )
         {
            getline( f, temp );
            stringstream ss( temp );
            string token;
            vector<int> temp_v;
            while ( getline( ss, token, ',' ) )
            {
               //cout << stoi( token ) << " ";
               temp_v.emplace_back( stoi( token ) );
            }
            //cout << endl;
            if ( token.empty() )
            {
               continue;
            }
            Vector3i vvv( temp_v[0], temp_v[1], temp_v[2] );
            t_s.point_vector.push_back( vvv );
         }
         scan_v.push_back( t_s );
      }
   }

   /*for ( auto &s : scan_v )
   {
      cout << s.scanner_id << endl;
      for ( auto &vec : s.point_vector )
      {
         cout << vec.format( CleanFmt ) << endl << endl;
      }
      cout << s.point_vector.size();
      cout << endl;
   }*/

}

void generateRelSets()
{
   // generates the first one
   for ( auto &sc : scan_v )   // for each scanner
   {
      for ( auto &k : matSet ) // for each orientation
      {
         set<point_3d> ret;
         for ( int i = 0; i < sc.point_vector.size(); ++i )    // for each point
         {
            for ( int j = 0; j < sc.point_vector.size(); ++j ) // for each other point
            {
               if ( i != j )
               {
                  auto b1 = sc.point_vector[i];                                                     // get the first point
                  auto b2 = sc.point_vector[j];                                                     // get the second point

                  b1 = k * b1;                                                                      // apply the orientation
                  b2 = k * b2;                                                                      // apply the orientation

                  auto rel = make_tuple( b2( 0 ) - b1( 0 ), b2( 1 ) - b1( 1 ), b2( 2 ) - b1( 2 ) ); // compute relative distance
                  //cout << get<0>( rel ) << " " << get<1>( rel ) << " " << get<2>( rel ) << endl;
                  ret.insert( rel );                                                                // insert into set
               }
            }
         }
         assert( ret.size() == ( sc.point_vector.size() * ( sc.point_vector.size() - 1 ) ) ); // (n(n-1) unique pairs where order matters)
         sc.rel_pos_sets.push_back( ret );                                                    // push the set back
      }
      assert( sc.rel_pos_sets.size() == 24 );
      cout << sc.scanner_id << " locked and loaded" << endl;
   }

}

void findOverLaps()
{
   for ( int i = 0; i < scan_v.size(); ++i )
   {
      for ( int j = 0; j < scan_v.size(); ++j )
      {
         if ( i != j )
         {
            auto &scanner0 = scan_v[i];
            auto &scanner1 = scan_v[j];
            for ( auto &set1 : scanner0.rel_pos_sets ) // for first rotation set
            {
               bool foundOne = false;
               for ( auto &set2 : scanner1.rel_pos_sets ) // for second rotations set
               {
                  vector<point_3d> v_out( set1.size() + set2.size() );

                  auto it = set_intersection( set1.begin(), set1.end(), set2.begin(), set2.end(), v_out.begin() );

                  v_out.resize( it - v_out.begin() );

                  if ( v_out.size() >= 132 ) // n(n-1)
                  {
                     cout << "overlap found" << v_out.size() << endl;
                     foundOne = true;
                     for ( auto &olap : v_out )
                     {
                        all_overlaps.insert( olap );
                     }
                     break;
                  }

               }
               if ( foundOne )
               {
                  break;
               }
            }
         }
      }
   }
   cout << all_overlaps.size();
}

void partOne()
{
   generateRelSets();

   findOverLaps();

}

int main()
{
   generateRotMatrices(); // we now have a set of the rotation matrices

   parseInput();
   partOne();
}

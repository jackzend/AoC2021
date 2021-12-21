#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <array>
#include <eigen3/Eigen/Eigen>

using namespace std;

using namespace Eigen;

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
   cout << "Num Mats: " << matSet.size();
}

int main()
{
   generateRotMatrices();
}

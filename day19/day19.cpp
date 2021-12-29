#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <array>

//#include <eigen3/Eigen/Eigen> //if on work machine
#include <Eigen>
#include <set>
#include <sstream>
#include <string>
#include <cassert>
#include <utility>
#include <stack>

using namespace std;

using namespace Eigen;

using point_3d = tuple<int, int, int>;

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

template <typename T>
struct matrix_hash : std::unary_function<T, size_t>
{
   std::size_t operator()(T const &matrix) const
   {
      // Note that it is oblivious to the storage order of Eigen matrix (column- or
      // row-major). It will give you the same hash value for two different matrices if they
      // are the transpose of each other in different storage order.
      size_t seed = 0;
      for (size_t i = 0; i < matrix.size(); ++i)
      {
         auto elem = *(matrix.data() + i);
         seed ^= std::hash<typename T::Scalar>()(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
   }
};
using rotation_to_points_map = unordered_map<Matrix3i, vector<Vector3i>, matrix_hash<Matrix3i>>;

unordered_set<pair<int, int>, hash_pair> matched_map;

unordered_set<Vector3i, matrix_hash<Vector3i>> FINAL_SET;
//unordered_map<int, rotation_to_points_map> database;

unordered_map<int, set<point_3d>> known_values_map;

struct Scanner
{
   int id;
   rotation_to_points_map rot_points; // key is rotation matrix, value is the rotated points

   bool isMatched;                     // if we find a match, we lock it
   Matrix3i matched_mat;               // the transform matrix we use to get the lock
   vector<Vector3i> matched_points;    // the locked points
   set<point_3d> matched_rel_dist_set; // the locked relative distance set

   Vector3i matched_transform_from_0;

   unordered_map<int, point_3d> point_transformations; // ex if its 1 : (5,0) that means //from this scanner to another scanner, apply the transofrm to get the other points in //
   //reference to this scanner

   point_3d tranform_to_zero; // apply this to get the (transformed) points in reference to zero
};

vector<Scanner> database;
IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

Matrix3i genMatrix(double roll, double yaw, double pitch)
{
   AngleAxisd rollAngle(roll, Vector3d::UnitZ());
   AngleAxisd yawAngle(yaw, Vector3d::UnitY());
   AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitX());

   Quaternion<double> q = rollAngle * yawAngle * pitchAngle;

   return q.matrix().cast<int>();
}

unordered_set<Matrix3i, matrix_hash<Matrix3i>> matSet; // good

tuple<int, int, int> toTuple(Vector3i &eig_v)
{
   tuple<int, int, int> ret(eig_v(0), eig_v(1), eig_v(3));
   return ret;
}

Vector3i toEigenVector(point_3d &tup)
{
   Vector3i ret(get<0>(tup), get<1>(tup), get<2>(tup));
   return ret;
}

Matrix3i ID_MATRIX = Matrix3i::Identity();

void generateRotMatrices()
{
   vector<double> deg{0, M_PI / 2, M_PI, 3 * (M_PI / 2)};
   for (int i = 0; i < 4; ++i)
   {
      for (int j = 0; j < 4; ++j)
      {
         for (int k = 0; k < 4; ++k)
         {
            auto mat = genMatrix(deg[k], deg[j], deg[i]);

            if (mat.determinant() == 1)
            {
               matSet.insert(mat);
            }
         }
      }
   }
   /*for (auto &k : matSet)
   {
      cout << k.format(CleanFmt) << endl
           << endl;
   }
   cout << "Num Mats: " << matSet.size() << endl;*/
}

void parseInput()
{
   ifstream f;
   f.open("day19.txt");

   string temp;
   int scanner_num = 0;
   while (getline(f, temp))
   {
      Scanner temp_scan;
      if (temp[0] == '-' and temp[1] == '-')
      {

         temp_scan.id = scanner_num;
         rotation_to_points_map empty_map;
         temp_scan.rot_points = empty_map;
         vector<Vector3i> empty_vec;

         for (auto &mat : matSet)
         {
            temp_scan.rot_points[mat] = empty_vec;
            //database[scanner_num][mat] = empty_vec;
         }

         while (!temp.empty())
         {
            getline(f, temp);
            stringstream ss(temp);
            string token;
            vector<int> temp_v;
            while (getline(ss, token, ','))
            {
               //cout << stoi( token ) << " ";
               temp_v.emplace_back(stoi(token)); // tempv has the numbers
            }
            //cout << endl;
            if (token.empty())
            {
               continue;
            }
            int xval = temp_v[0];
            int yval = temp_v[1];
            int zval = temp_v[2];
            for (auto &mat : matSet)
            {

               Vector3i t_v(xval, yval, zval);
               t_v = mat * t_v;
               temp_scan.rot_points[mat].push_back(t_v);
               //database[scanner_num][mat].push_back(t_v);
            }
         }
         temp_scan.isMatched = false;
         database.push_back(temp_scan);
         ++scanner_num;
      }
   }
}

void printDataBase()
{
   cout << endl;
   cout << "---- Printing DataBase ----" << endl;
   cout << endl;
   for (auto &scanner : database)
   {
      cout << "Scanner ID #: " << scanner.id << endl
           << endl;
      for (auto &[mat, vec] : scanner.rot_points)
      {
         cout << "-- Translation Matrix --" << endl;
         cout << mat.format(CleanFmt) << endl
              << endl;

         for (auto &elt : vec)
         {
            cout << elt.transpose().format(CleanFmt) << endl;
         }
         cout << endl;
      }
   }
}

void generateRelSets()
{
}

void findOverLaps()
{
}

set<point_3d> getRelativeDifferenceSet(int id, Matrix3i &trans_key)
{
   auto &vec = database[id].rot_points[trans_key];
   set<point_3d> ret;
   for (int i = 0; i < vec.size(); ++i)
   {
      auto p1 = vec[i];
      for (int j = 0; j < vec.size(); ++j)
      {
         if (i != j)
         {
            auto p2 = vec[j];
            auto ins = make_tuple(p2(0) - p1(0), p2(1) - p1(1), p2(2) - p1(2));
            ret.insert(ins);
         }
      }
   }
   return ret;
}

pair<bool, vector<point_3d>> matchFound(set<point_3d> lhs, set<point_3d> rhs)
{
   vector<point_3d> set_int(lhs.size() + rhs.size());

   auto it = set_intersection(begin(lhs), end(lhs), begin(rhs), end(rhs), begin(set_int));

   set_int.resize(it - begin(set_int));

   if (set_int.size() >= 132)
   {
      return make_pair(true, set_int);
   }
   vector<point_3d> empty_ret;
   return make_pair(false, empty_ret);
}

Vector3i further_processing(int id1, int id2, Matrix3i key, vector<point_3d> &test_rel) // key is for id2
{
   //cout << "Processing Further..." << endl;

   Vector3i a1(-1, -1, -1), b1(-1, -1, -1), a2(-1, -1, -1), b2(-1, -1, -1); // these are the points we want to find

   auto tester = test_rel[10];
   for (int i = 0; i < database[id1].matched_points.size(); ++i)
   {
      for (int j = 0; j < database[id1].matched_points.size(); ++j)
      {
         if (i != j)
         {

            auto first1 = database[id1].matched_points[i];
            auto first2 = database[id1].matched_points[j];

            if ((first2 - first1) == toEigenVector(tester))
            {
               a1 = first1;
               b1 = first2;
            }

            auto second1 = database[id2].rot_points[key][i];
            auto second2 = database[id2].rot_points[key][j];

            if ((second2 - second1) == toEigenVector(tester))
            {
               a2 = second1;
               b2 = second2;
            }
         }
      }
   }

   auto transformer = a2 - a1;

   /*cout << "LOCATION OF SCANNER " << id2 << " IN REFERENCE TO " << id1 << " IS \n"
        << (a1 - a2).format(CleanFmt) << endl;*/

   return (a1 - a2);
   //cout << transformer.format(CleanFmt) << endl;
}

void partOne()
{
   stack<int> scanner_stack;
   scanner_stack.push(0);
   set<int> visited;
   visited.insert(0);
   database[0].matched_mat = ID_MATRIX; // lock this tranformation
   database[0].isMatched = true;        // the first one is automatically locked

   database[0].matched_points = database[0].rot_points[ID_MATRIX];
   database[0].matched_rel_dist_set = getRelativeDifferenceSet(0, ID_MATRIX);
   Vector3i t_v3i(0, 0, 0);
   database[0].matched_transform_from_0 = t_v3i;

   while (!scanner_stack.empty())
   {
      //cout << " IN HERE" << endl;
      auto curr_id = scanner_stack.top();
      scanner_stack.pop();
      auto curr = database[curr_id]; // curr is guranteed to be locked
      auto curr_rel_diff = curr.matched_rel_dist_set;

      for (int i = 0; i < database.size(); ++i) // generate potential neighbors
      {
         if (curr_id != i and visited.find(i) == visited.end()) // valid neighbor
         {
            auto neighbor = database[i];

            for (auto &[mat, vec] : neighbor.rot_points) // find the match
            {
               auto key = mat;
               auto comp_rel_diff = getRelativeDifferenceSet(i, key);
               auto check = matchFound(curr_rel_diff, comp_rel_diff);
               if (check.first)
               {
                  //cout << "Match Found: " << curr_id << " : " << i << endl;
                  neighbor.isMatched = true;
                  neighbor.matched_points = neighbor.rot_points[key];
                  neighbor.matched_rel_dist_set = comp_rel_diff;
                  neighbor.matched_mat = key;
                  auto intermediary_transform = further_processing(curr_id, i, key, check.second);

                  neighbor.matched_transform_from_0 = database[curr_id].matched_transform_from_0 + intermediary_transform;
                  cout << "Location of Scanner " << i << " from scanner 0: \n " << neighbor.matched_transform_from_0.format(CleanFmt) << endl;
                  scanner_stack.push(i);
                  visited.insert(i);
                  database[i] = neighbor;
                  break;
               }
            }
         }
      }
   }

   for (auto &scanner : database)
   {
      for (auto &pt : scanner.matched_points)
      {
         Vector3i new_point = pt + scanner.matched_transform_from_0;
         FINAL_SET.insert(new_point);
      }
   }

   cout << FINAL_SET.size() << endl;

   int max_d = 0;
   for (auto &scanner : database)
   {
      for (auto &scanner1 : database)
      {
         if (scanner.id != scanner1.id)
         {
            auto temp = scanner1.matched_transform_from_0 - scanner.matched_transform_from_0;

            int t = abs(temp(0)) + abs(temp(1)) + abs(temp(2));
            if (t > max_d)
            {
               max_d = t;
            }
         }
      }
   }
   cout << max_d << endl;
   /*for (auto &scanner : database) // for each scanner ID basically
   {
      int first_id = scanner.id;
      set<point_3d> orig_rel_diff;
      if (first_id == 0)
      {
         orig_rel_diff = getRelativeDifferenceSet(first_id, ID_MATRIX); // want to get everything locked with scanner 0
      }
      else
      {
         orig_rel_diff = getRelativeDifferenceSet(first_id, scanner.matched_mat);
      }

      for (auto &scanner2 : database)
      {
         if (scanner.id != scanner2.id)
         {
            int second_id = scanner2.id;
            for (auto &[mat, vec] : scanner2.rot_points)
            {
               auto key = mat;
               auto comp_rel_diff = getRelativeDifferenceSet(second_id, key);
               auto bool_set_int_pair = matchFound(orig_rel_diff, comp_rel_diff);
               if (bool_set_int_pair.first)
               {
                  auto check1 = make_pair(first_id, second_id);
                  auto check2 = make_pair(second_id, first_id);

                  if (true)
                  {
                     cout << "Match Found: Scanners " << first_id << " : " << second_id << endl;
                     matched_map.insert(make_pair(first_id, second_id));
                     further_processing(first_id, second_id, key, bool_set_int_pair.second[0]);
                     scanner2.matched_mat = mat;
                  }
               }
            }
         }
      }
   }*/
}

int main()
{
   generateRotMatrices(); // we now have a set of the rotation matrices

   parseInput();
   //printDataBase();
   partOne();
}

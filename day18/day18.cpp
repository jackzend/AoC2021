#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

struct Node
{
   int  data;
   bool isSingleValue;

   Node *left;
   Node *right;

   Node *prev;

   Node( int val )
   {
      data = val;

      // Left and right child for node
      // will be initialized to null
      left  = nullptr;
      right = nullptr;
      prev  = nullptr;
   }
};

queue<Node *> node_v;

vector<Node *> part2_v;

Node *createBasePair( int l,
                      int r );
Node *createPair( Node *ln,
                  Node *rn );
Node *createSingleValueNode( int v );
void printTree( Node *n );
void writeTree( Node   *n,
                string &str );
Node *buildTree( string &str );
void explodeNode( Node *n );
void splitNode( Node *n );
void explodeNode( Node *n,
                  Node *root );

Node *deepCopy( Node *source )
{
   string temp;
   writeTree( source, temp );
   auto ret = buildTree( temp );
   return ret;
}

Node *getFirstExplode( Node *root );
Node *getFirstSplit( Node *root );  // this works

void fixInvariants( Node *root );

void parseInput( string fn )
{
   ifstream f;
   f.open( fn );
   string temp;
   while ( getline( f, temp ) )
   {
      Node *temp_node = buildTree( temp );

      node_v.push( temp_node );
      part2_v.push_back( temp_node );
   }
}

int isTooDeep( Node *n, Node *root )
{
   if ( root == nullptr )
   {
      return -1;
   }

   int dist = -1;

   if ( ( root == n ) or ( dist = isTooDeep( n, root->left ) ) >= 0 or ( dist = isTooDeep( n, root->right ) ) >= 0 )
   {
      return dist + 1;
   }
   return dist;

}

bool fixInvariants_impl( Node *root ) // returns false once we dont find an invariant
{
}

void fixInvariants( Node *root )
{
}

void fix_tree( Node *root )
{
}

bool treeIsGood( Node *root )
{
   if ( getFirstSplit( root ) == nullptr and getFirstExplode( root ) == nullptr )
   {
      return true;
   }
   return false;
}

Node *getFirstSplit( Node *root )  // this works
{
   //cout << "We're actually in getFirstSplit()" << endl;
   stack<Node *> s;
   assert( root != nullptr );
   s.push( root );
   while ( !s.empty() )
   {
      auto curr = s.top();
      s.pop();
      if ( curr->isSingleValue )
      {
         //cout << "checking " << curr->data << endl;
         if ( curr->data >= 10 )
         {
            //cout << "Found a Splitter: ";
            //cout << curr->data << endl;
            return curr;
         }
      }
      else
      {
         //cout << "Can't find single value: " << endl;
         s.push( curr->right );
         s.push( curr->left );
      }

   }
   return nullptr;
}

Node *getFirstExplode( Node *root )  // returns the left most node that needs to blow up, if none then nullptr
{
   stack<Node *> s;
   s.push( root );

   while ( !s.empty() )
   {
      auto curr = s.top();
      s.pop();
      if ( !curr->isSingleValue )
      {
         if ( isTooDeep( curr, root ) == 4 )
         {
            //cout << "Found a Sploder: ";
            //cout << curr->left->data << " " << curr->right->data << endl;
            return curr;
         }
         s.push( curr->right );
         s.push( curr->left );
      }
   }
   return nullptr;

}

int getMagnitude( Node *root )
{
   if ( root->isSingleValue )
   {
      return root->data;
   }
   return 3 * getMagnitude( root->left ) + 2 * getMagnitude( root->right );
}

int runPart2()
{
   int max = 0;
   for ( int i = 0; i < part2_v.size(); ++i )
   {
      for ( int j = 0; j < part2_v.size(); ++j )
      {
         if ( i != j )
         {
            auto first  = deepCopy( part2_v[i] );
            auto second = deepCopy( part2_v[j] );
            auto root   = createPair( first, second );
            while ( !treeIsGood( root ) )
            {
               if ( getFirstExplode( root ) != nullptr )
               {
                  explodeNode( getFirstExplode( root ), root );
                  continue;
               }
               if ( getFirstSplit( root ) != nullptr )
               {
                  //cout << "Attempting to split?" << endl;
                  splitNode( getFirstSplit( root ) );
               }
            }
            int mag = getMagnitude( root );
            if ( mag > max )
            {
               max = mag;
            }

         }
      }
   }
   return max;
}

int runComputations()
{
   auto sum = node_v.front();
   node_v.pop();

   while ( !node_v.empty() )
   {
      auto add = node_v.front();
      node_v.pop();
      auto temp = createPair( sum, add );
      sum  = temp;
      temp = nullptr;

      //printTree( sum );

      while ( !treeIsGood( sum ) )
      {
         if ( getFirstExplode( sum ) != nullptr )
         {
            explodeNode( getFirstExplode( sum ), sum );
            continue;
         }
         if ( getFirstSplit( sum ) != nullptr )
         {
            //cout << "Attempting to split?" << endl;
            splitNode( getFirstSplit( sum ) );
         }
      }
   }

   cout << "final sum:" << endl;
   printTree( sum );
   cout << endl;

   string temp;
   writeTree( sum, temp );
   cout << " written string: " << temp << endl;

   auto tempRoot = buildTree( temp );
   cout << "rebuilt tree:" << endl;
   printTree( tempRoot );
   cout << endl;

   return getMagnitude( sum );
}

Node *buildTree( string &str )
{
   //cout << str << endl;
   if ( str.empty() )
   {
      return nullptr;
   }
   if ( str[0] == ']' )
   {
      //cout << "THIS FUCKS US" << endl;
      int i = 0;
      while ( str[i] == ']' )
      {
         ++i;
      }
      str.erase( begin( str ), begin( str ) + i + 1 ); // should eat up all the ']'

      if ( str[0] == ',' and str[1] == '[' )
      {
         str.erase( begin( str ), begin( str ) + 1 ); // erase the lone comma, leave it open for the ,X case
      }
   }

   if ( ( str[0] == ',' and str[1] == '[' ) )
   {
      str.erase( begin( str ), begin( str ) + 1 ); // erase lone comma
   }

   if ( str[0] == '[' and isdigit( str[1] ) and isdigit( str[3] ) ) // special case, base pair
   {
      char l = str[1];
      char r = str[3];

      int left  = l - '0';
      int right = r - '0';
      str.erase( begin( str ), begin( str ) + 5 ); // we eat 5 characters : [4,5]
      return createBasePair( left, right );
   }
   else if ( str[0] == ',' and isdigit( str[1] ) ) // this case is a pair has a single right value where the left is some compound pairing
   {
      int v = str[1] - '0';
      str.erase( begin( str ), begin( str ) + 3 ); // we eat ",X]"
      return createSingleValueNode( v );
   }
   else if ( isdigit( str[0] ) ) // single left value
   {
      int v = str[0] - '0';
      str.erase( begin( str ), begin( str ) + 2 ); // we eat "X,"
      return createSingleValueNode( v );
   }
   else if ( str[0] == '[' )
   {
      str.erase( begin( str ), begin( str ) + 1 ); // we eat 1 character
      Node *left  = buildTree( str );
      Node *right = buildTree( str );
      return createPair( left, right );
   }
}

void writeTree( Node *n, string &str )
{
   if ( n == nullptr )
   {
      return;
   }
   if ( n->data == -1 )
   {
      assert( n->isSingleValue == false );
      str += "[";
      writeTree( n->left, str );
      str += ",";
      writeTree( n->right, str );
      str += "]";
   }
   else
   {
      assert( n->isSingleValue == true );
      assert( n->left == nullptr );
      assert( n->right == nullptr );
      assert( n->prev != nullptr );
      str += to_string( n->data );
   }
}

void printTree( Node *n )
{
   if ( n == nullptr )
   {
      return;
   }
   if ( n->data == -1 )
   {
      assert( n->isSingleValue == false );
      cout << "[";
      printTree( n->left );
      cout << ",";
      printTree( n->right );
      cout << "]";
   }
   else
   {
      assert( n->isSingleValue == true );
      assert( n->left == nullptr );
      assert( n->right == nullptr );
      assert( n->prev != nullptr );
      cout << n->data;
   }
}

void flattenTree( Node *n, vector<Node *> &v ) // flattens the tree into an array of all the single value nodes, this is a bit cheeky
{
   if ( n == nullptr )
   {
      return;
   }
   if ( n->data == -1 )
   {
      assert( n->isSingleValue == false );
      //cout << "[";
      flattenTree( n->left, v );
      //cout << ",";
      flattenTree( n->right, v );
      //cout << "]";
   }
   else
   {
      assert( n->isSingleValue == true );
      assert( n->left == nullptr );
      assert( n->right == nullptr );
      assert( n->prev != nullptr );
      v.push_back( n );
      //cout << n->data;
   }
}

void explodeNode( Node *n, Node *root ) // this appears to be working lol // need to add a dfs traversal down the left and right subtrees
{
   vector<Node *> v;
   flattenTree( root, v );
   /* for ( auto &nd: v )
    {
       cout << nd->data << " ";
    }
    cout << endl;*/

   auto left_searcher  = n->left;
   auto right_searcher = n->right;

   for ( int i = 0; i < v.size(); ++i )
   {
      if ( v[i] == left_searcher )
      {
         if ( i != 0 )
         {
            v[i - 1]->data += left_searcher->data;
         }
      }
      if ( v[i] == right_searcher )
      {
         if ( i != v.size() - 1 )
         {
            v[i + 1]->data += right_searcher->data;
         }
      }
   }

   delete n->left;
   delete n->right;
   n->left          = nullptr;
   n->right         = nullptr;
   n->data          = 0;
   n->isSingleValue = true;

}

void splitNode( Node *n )
{
   int left_n, right_n;
   if ( n->data % 2 == 0 )
   {
      left_n = right_n = n->data / 2;
   }
   else
   {
      left_n  = n->data / 2;
      right_n = ( n->data / 2 ) + 1;
   }

   n->data          = -1;
   n->isSingleValue = false;
   n->left          = createSingleValueNode( left_n );
   n->right         = createSingleValueNode( right_n );
   n->left->prev    = n;
   n->right->prev   = n;
}

Node *createBasePair( int l, int r )
{
   Node *n = new Node( -1 );

   n->left  = new Node( l );
   n->right = new Node( r );

   n->left->prev  = n;
   n->right->prev = n;

   n->left->isSingleValue  = true;
   n->right->isSingleValue = true;

   return n;
}

Node *createPair( Node *ln, Node *rn )
{
   Node *n = new Node( -1 );

   n->left  = ln;
   n->right = rn;

   rn->prev         = n;
   ln->prev         = n;
   n->prev          = nullptr;
   n->isSingleValue = false;

   return n;
}

Node *createSingleValueNode( int v )
{
   Node *ret = new Node( v );
   ret->isSingleValue = true;
   return ret;
}

int main()
{
   //parseInput( "day18_test.txt" );
   //runComputations();
   /*string temp  = "[[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[1,9],[6,9]],[[8,2],[7,3]]]],[2,4]";
   Node   *root = buildTree( temp );
   printTree( root );
   cout << endl;
   //assert( isNoExplodes( root ) == true ); // forcing to print all illegal pairs
   //assert( isNoSplits( root ) == true );

   assert( getFirstExplode( root ) != nullptr );
   explodeNode( getFirstExplode( root ), root );

   printTree( root );
   cout << endl;

   explodeNode( getFirstExplode( root ), root );
   printTree( root );
   cout << endl;

   explodeNode( getFirstExplode( root ), root );
   printTree( root );
   cout << endl;

   explodeNode( getFirstExplode( root ), root );
   printTree( root );
   cout << endl;

   splitNode( getFirstSplit( root ) );
   printTree( root );
   cout << endl;

   explodeNode( getFirstExplode( root ), root );
   printTree( root );
   cout << endl;*/
   parseInput( "day18_test.txt" );
   //cout << runComputations() << endl;
   cout << runPart2() << endl;
}

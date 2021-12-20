#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <stack>

using namespace std;

struct Node
{
    int data;
    bool isSingleValue;

    Node *left;
    Node *right;

    Node *prev;

    Node(int val)
    {
        data = val;

        // Left and right child for node
        // will be initialized to null
        left = nullptr;
        right = nullptr;
        prev = nullptr;
    }
};

stack<Node *> node_v;

Node *createBasePair(int l, int r);
Node *createPair(Node *ln, Node *rn);
Node *createSingleValueNode(int v);
void printTree(Node *n);
Node *buildTree(string &str);
void explodeNode(Node *n);
void splitNode(Node *n);

void fixInvariants(Node *root);

void parseInput(string fn)
{
    ifstream f;
    f.open(fn);
    string temp;
    while (getline(f, temp))
    {
        Node *temp_node = buildTree(temp);

        node_v.push(temp_node);
    }
}

bool isTooDeep(Node *n)
{
    assert(n != nullptr);
    cout << "is too deep got called" << endl;
    auto curr = n;
    int d = 0;
    while (curr->prev != nullptr)
    {
        curr = curr->prev;
        ++d;
    }

    if (d == 4 and !n->isSingleValue)
    {
        explodeNode(n);
        return true;
    }
    return false;
}

bool isTooBig(Node *n)
{
    if (n->isSingleValue and n->data >= 10)
    {
        splitNode(n);
        return true;
    }
    return false;
}

bool fixInvariants_impl(Node *root) // returns false once we dont find an invariant
{
    //bool exhuasted_search = false;
    stack<Node *> s;
    Node *curr = root;

    while (curr != nullptr or s.empty() == false)
    {
        while (curr != nullptr)
        {
            cout << "make it here a few times" << endl;
            s.push(curr);
            if (curr->isSingleValue)
            {
                break;
            }
            curr = curr->left;
        }
        curr = s.top();
        s.pop();
        if (isTooDeep(curr) or isTooBig(curr))
        {
            return true;
        }
        if (!curr->isSingleValue)
        {
            curr = curr->right;
        }
        else
        {
            curr = nullptr;
        }
    }
    return false;
}

void fixInvariants(Node *root)
{
    while (fixInvariants_impl(root))
    {
        cout << "fixed one" << endl;
    }
}

int runComputations()
{
    Node *sum = node_v.top();
    node_v.pop();
    while (!node_v.empty())
    {
        auto lhs = node_v.top();
        node_v.pop();
        sum = createPair(lhs, sum); // this is the addition step

        printTree(sum);
        cout << endl;

        fixInvariants(sum);
    }

    printTree(sum);
    cout << endl;
    return -1;
}

Node *buildTree(string &str)
{
    //cout << str << endl;
    if (str.empty())
    {
        return nullptr;
    }
    if (str[0] == ']')
    {
        //cout << "THIS FUCKS US" << endl;
        int i = 0;
        while (str[i] == ']')
        {
            ++i;
        }
        str.erase(begin(str), begin(str) + i + 1); // should eat up all the ']'

        if (str[0] == ',' and str[1] == '[')
        {
            str.erase(begin(str), begin(str) + 1); // erase the lone comma, leave it open for the ,X case
        }
    }

    if ((str[0] == ',' and str[1] == '['))
    {
        str.erase(begin(str), begin(str) + 1); // erase lone comma
    }

    if (str[0] == '[' and isdigit(str[1]) and isdigit(str[3])) // special case, base pair
    {
        char l = str[1];
        char r = str[3];

        int left = l - '0';
        int right = r - '0';
        str.erase(begin(str), begin(str) + 5); // we eat 5 characters : [4,5]
        return createBasePair(left, right);
    }

    else if (str[0] == ',' and isdigit(str[1])) // this case is a pair has a single right value where the left is some compound pairing
    {
        int v = str[1] - '0';
        str.erase(begin(str), begin(str) + 3); // we eat ",X]"
        return createSingleValueNode(v);
    }

    else if (isdigit(str[0])) // single left value
    {
        int v = str[0] - '0';
        str.erase(begin(str), begin(str) + 2); // we eat "X,"
        return createSingleValueNode(v);
    }

    else if (str[0] == '[')
    {
        str.erase(begin(str), begin(str) + 1); // we eat 1 character
        return createPair(buildTree(str), buildTree(str));
    }
}

void printTree(Node *n)
{
    if (n == nullptr)
        return;
    if (n->data == -1)
    {
        cout << "[";
        printTree(n->left);
        cout << ",";
        printTree(n->right);
        cout << "]";
    }
    else
    {
        cout << n->data;
    }
}

void explodeNode(Node *n) // this appears to be working lol // need to add a dfs traversal down the left and right subtrees
{
    cout << "EXPLODING" << endl;
    int left_i = n->left->data;
    int right_i = n->right->data;
    cout << left_i << " " << right_i << endl;

    delete n->right;
    delete n->left;

    n->data = 0;
    n->isSingleValue = true;
    // now we need to perform a back tracking left search
    Node *curr = n->prev;
    assert(curr != nullptr);
    Node *last = n;
    while (curr != nullptr) // end at the root
    {
        //cout << "in Loop" << endl;
        if (curr->left != last and curr->left != nullptr and curr->left->isSingleValue)
        {
            cout << " we shouldnt be in here";
            curr->left->data += left_i;
            break;
        }
        last = curr;
        curr = curr->prev;
    }
    cout << " made it to here " << endl;
    printTree(last);
    cout << endl;
    Node *curr2 = n->prev;
    cout << " seg fault here??" << endl;
    Node *last1 = n;
    while (curr2 != nullptr) // end at the root
    {
        if (curr2->right != last1 and curr2->right != nullptr and curr2->right->isSingleValue)
        {
            curr2->right->data += right_i;
            //cout << curr->right->data << endl;
            break;
        }
        last1 = curr2;
        curr2 = curr2->prev;
    }
    cout << " we made it here somehow" << endl;
    printTree(last);
    cout << endl;
}

void splitNode(Node *n)
{
    cout << "splitting" << endl;
    assert(n->left == nullptr);
    assert(n->right == nullptr);

    int left_split, right_split;

    if (n->data % 2 == 0) // even
    {
        cout << "In Here" << endl;
        left_split = right_split = (n->data / 2);
    }
    else
    {
        left_split = (n->data / 2);
        right_split = (n->data / 2) + 1;
    }
    assert(left_split + right_split == n->data);

    Node *prev = n->prev;
    delete n;
    n = createBasePair(left_split, right_split);
    n->prev = prev;
    n->data = -1;
    n->isSingleValue = false;
}

Node *createBasePair(int l, int r)
{
    Node *n = new Node(-1);

    n->left = new Node(l);
    n->right = new Node(r);

    n->left->prev = n;
    n->right->prev = n;

    n->left->isSingleValue = true;
    n->right->isSingleValue = true;

    return n;
}

Node *createPair(Node *ln, Node *rn)
{
    Node *n = new Node(-1);

    n->left = ln;
    n->right = rn;

    rn->prev = n;
    ln->prev = n;

    return n;
}

Node *createSingleValueNode(int v)
{
    Node *ret = new Node(v);
    ret->isSingleValue = true;
    return ret;
}

int main()
{
    /*Node *root = new Node(-1);

    root->left = new Node(1);
    root->right = new Node(2);

    printTree(root);
    cout << endl;

    Node *newRoot = new Node(-1);

    newRoot->left = root;
    newRoot->right = new Node(3);

    printTree(newRoot);
    cout << endl;*/

    /* Node *r1 = createPair(createPair(createBasePair(1, 3), createBasePair(5, 3)), createPair(createBasePair(1, 3), createBasePair(8, 7)));
    Node *r2 = createPair(createPair(createBasePair(4, 9), createBasePair(6, 9)), createPair(createBasePair(8, 2), createBasePair(7, 3)));

    r1 = createPair(r1, r2);
    printTree(r1);
    cout << endl;
    Node *root = createBasePair(13, 1);

    splitNode(root->left);
    printTree(root);
    cout << endl;
    explodeNode(root->left);
    printTree(root);
    cout << endl;*/
    /*string temp = string("[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]");
    Node *r1 = buildTree(temp);

    printTree(r1);
    cout << endl;

    string temp1 = string("[[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]");
    Node *r2 = buildTree(temp1);

    printTree(r2);
    cout << endl;*/
    //parseInput("day18_test.txt");
    //runComputations();
    string one = string("[[[[4,3],4],4],[7,[[8,4],9]]]");
    Node *r1 = buildTree(one);
    printTree(r1);
    cout << endl;
    string two = string("[1,1]");
    Node *r2 = buildTree(two);
    printTree(r2);
    cout << endl;

    node_v.push(r1);
    node_v.push(r2);
    runComputations();
}
#include <iostream>
#include <queue>
#include <stack>
#include <string>

using namespace std;

class NotImplemented : public logic_error
{
public:
    NotImplemented() : logic_error(" Not implemented \n") { }
};

class WrongAnswer : public logic_error
{
public:
    WrongAnswer() : logic_error(" Wrong answer \n") { }
};

struct node
{
    char symbol;
    node* left;
    node* right;
};

class Tree
{
private:
    node* root;

    string _preorder(node* ptr)
    {
        if (ptr)
            return string(1, ptr->symbol) + _preorder(ptr->left) + _preorder(ptr->right);
        else
            return "";
    }

    string _inorder(node* ptr)
    {
        if (ptr)
            return _inorder(ptr->left) + string(1, ptr->symbol) + _inorder(ptr->right);
        else
            return "";
    }

public:
    Tree()
    {
        root = nullptr;
    }

    // Here we assume that symbols are unique
    // pretext = symbols in 'preorder' order
    // intext = symbols in 'inorder' order
    Tree(string pretext, string intext)
    {
        throw NotImplemented();
    }
    
    Tree(const string &text)
    {
        stack<node*> s;
        int i = 0;
        int len = text.length();
        while (i < len)
        {
            if (text[i] != '(')
            {
                node* n = new node;
                n->symbol = text[i];
                n->left = nullptr;
                n->right = nullptr;
                s.push(n);
                ++i;
            }
            else
            {
                node* n = new node;
                n->symbol = text[i + 1];
                n->right = s.top();
                s.pop();
                n->left = s.top();
                s.pop();
                s.push(n);
                i += 3;
            }
        }
        if (len > 0)
        {
            root = s.top();
            s.pop();
        }
        else
        {
            root = nullptr;
        }
    }
    
    ~Tree()
    {
        queue<node*> q;
        q.push(root);
        while (!q.empty())
        {
            node* n = q.front();
            q.pop();
            if (n)
            {
                q.push(n->left);
                q.push(n->right);
                delete n;
            }
        }
    }
    
    string preorder()
    {
        return _preorder(root);
    }

    string inorder()
    {
        return _inorder(root);
    }

    // Here we treat this tree as binary search tree
    void insert(char key)
    {
        if (!root)
        {
            root = new node;
            root->symbol = key;
            root->left = nullptr;
            root->right = nullptr;
        }
        else
        {
            node* parent = nullptr;
            node* ptr = root;
            while (ptr)
            {
                if (ptr->symbol == key) return;
                parent = ptr;
                ptr = ptr->symbol < key ? ptr->right : ptr->left;
            }
            node* n = new node;
            n->symbol = key;
            n->left = nullptr;
            n->right = nullptr;
            if (parent->symbol < key)
                parent->right = n;
            else
                parent->left = n;
        }
    }
    
    // Determine the number of nodes in this tree
    int count()
    {
        throw NotImplemented();
    }

    // This tree as a string. See the test cases.
    string toString()
    {
        throw NotImplemented();
    }
};

void myAssert(string input, string answer, string expected)
{
    cout << input << " -> " << answer;
    if (answer != expected)
    {
        cout << "\n";
        throw WrongAnswer();
    }
    cout << " OK \n";
}

void testExe1()
{
    cout << "\nTesting count method..." << endl;
    try
    {
        myAssert("The empty tree", to_string(Tree().count()), "0");
        string input = "G";
        myAssert(input, to_string(Tree(input).count()), "1");
        input = "AB(P)EF(M)G(M)(S)";
        myAssert(input, to_string(Tree(input).count()), "9");
        input = "BDFG(E)(C)(A)";
        myAssert(input, to_string(Tree(input).count()), "7");
        Tree t;
        t.insert('A');
        t.insert('B');
        myAssert("BST(A,B)", to_string(t.count()), "2");
        cout << "Exe. 1 passed" << endl;
    }
    catch (logic_error &error)
    {
        cout << "Exe. 1 " << error.what() << endl;
    }
}

void testExe2()
{
    cout << "\nTesting toString method..." << endl;
    try
    {
        myAssert("The empty tree", Tree().toString(), "nil");
        string input = "G";
        myAssert(input, Tree(input).toString(), "G");
        input = "AB(P)EF(C)G(M)(S)";
        myAssert(input, Tree(input).toString(), "((A,P,B),S,((E,C,F),M,G))");
        input = "BDFG(E)(C)(A)";
        myAssert(input, Tree(input).toString(), "(B,A,(D,C,(F,E,G)))");
        Tree t1;
        t1.insert('B');
        t1.insert('A');
        myAssert("BST(B,A)", t1.toString(), "(A,B,nil)");
        Tree t2;
        t2.insert('D');
        t2.insert('A');
        t2.insert('C');
        myAssert("BST(D,A,C)", t2.toString(), "((nil,A,C),D,nil)");
        cout << "Exe. 2 passed" << endl;
    }
    catch (logic_error &error)
    {
        cout << "Exe. 2 " << error.what() << endl;
    }
}

void testExe3()
{
    cout << "\nTesting the second constructor..." << endl;
    try
    {
        myAssert("The empty tree", Tree("", "").toString(), "nil");
        Tree t1("G");
        string pre = t1.preorder();
        string in = t1.inorder();
        myAssert(pre + " " + in, Tree(pre, in).toString(), "G");
        Tree t2("AB(P)EF(C)G(M)(S)");
        pre = t2.preorder();
        in = t2.inorder();
        myAssert(pre + " " + in, Tree(pre, in).toString(), "((A,P,B),S,((E,C,F),M,G))");
        Tree t3("BDFG(E)(C)(A)");
        pre = t3.preorder();
        in = t3.inorder();
        myAssert(pre + " " + in, Tree(pre, in).toString(), "(B,A,(D,C,(F,E,G)))");
        Tree t4;
        t4.insert('B');
        t4.insert('A');
        pre = t4.preorder();
        in = t4.inorder();
        myAssert(pre + " " + in, Tree(pre, in).toString(), "(A,B,nil)");
        Tree t5;
        t5.insert('D');
        t5.insert('A');
        t5.insert('C');
        pre = t5.preorder();
        in = t5.inorder();
        myAssert(pre + " " + in, Tree(pre, in).toString(), "((nil,A,C),D,nil)");
        cout << "Exe. 3 passed" << endl;
    }
    catch (logic_error &error)
    {
        cout << "Exe. 3 " << error.what() << endl;
    }
}

int main(int argc, char* argv[])
{
    testExe1();
    testExe2();
    testExe3();
    return 0;
}

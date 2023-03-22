#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool TwoareEqual (Node* L, Node* R)
{
    // check if they are leave:
    if (L->left == nullptr && L->right == nullptr)
    {
        return (R->left == nullptr && R->right == nullptr);
    }
    if (R->left == nullptr && R->right == nullptr)
    {
        return (L->left == nullptr && L->right == nullptr);
    }

    //Other Cases

    if (L->left != nullptr)
    {
        if (L->right != nullptr)
        {
            if (!TwoareEqual(L->left, L->right)) return false;
        }
        if (R->left != nullptr)
        {
            if (!TwoareEqual(L->left, R->left)) return false;
        }
        else 
        {
            if (!TwoareEqual(L->left, R->right)) return false;
        }
    }
    else //L->right exist L->left does not
    {
        if (R->left != nullptr)
        {
            if (!TwoareEqual(L->right, R->left)) return false;
        }
        else 
        {
            if (!TwoareEqual(L->right, R->right)) return false;
        }
    }
    if (R->left != nullptr && R->right != nullptr)
    {
        if (!TwoareEqual(R->left, R->right)) return false;
    }
    return true;
}

bool equalPaths(Node * root)
{
    if (root == nullptr)
    {
        return true;
    }

    if (root->left == nullptr && root->right == nullptr)
    {
        return true;
    }
    else if (root->left == nullptr)
    {
        return equalPaths(root -> right);
    }
    else if (root->right == nullptr)
    {
        return equalPaths(root->left);
    }
    else //have two children
    {
        return TwoareEqual(root->left, root->right);
    }

}


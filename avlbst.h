#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void RecursiveInsertFix ( AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
		void RecursiveRemoveFix ( AVLNode<Key,Value>* n, int diff);
		void RotateRight (AVLNode<Key,Value>* p);
		void RotateLeft (AVLNode<Key,Value>* p);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  AVLNode< Key, Value>* Current = static_cast<AVLNode<Key,Value>*>(this->root_); 
		if (Current == nullptr)
		{
			this->root_ = new AVLNode<Key, Value>(new_item.first, 
			new_item.second, nullptr);
			return;
		}
    while (true)
    {			
      if (Current ->getItem().first == new_item.first)
      {
        Current -> setValue(new_item.second);
        break;
      }
      else if (Current -> getItem().first > new_item.first)
      {   
				if (Current->getLeft() == nullptr)
				{
					Current->setLeft(new AVLNode<Key, Value>(new_item.first, 
					new_item.second, Current));
					Current->updateBalance(-1);
					AVLNode<Key, Value>* n = Current->getLeft();
					if (Current->getBalance() == 0) return;
					else RecursiveInsertFix(Current, n);
					break;
				}
				else 
				{
					Current = Current->getLeft();
				}
      }
			else 
			{
				if (Current->getRight() == nullptr)
				{
					Current->setRight(new AVLNode<Key, Value>(new_item.first, 
					new_item.second, Current));
					Current->updateBalance(1);
					AVLNode<Key, Value>* n = Current->getRight();
					if (Current->getBalance() == 0) return;
					else RecursiveInsertFix(Current, n);
					break;
				}
				else 
				{
					Current = Current->getRight();
				}
			}
    }

/*
    AVLNode<Key, Value>* parent = Current->getParent(); 
		if (parent == nullptr) return;
    if (parent->getBalance() == -1 || parent->getBalance() == 1)
    {
        parent->setBalance(0);
        return;
    }
    if (parent->getLeft() == Current) parent->setBalance(-1);
    else parent->setBalance(1);
    RecursiveInsertFix(parent, Current);
	*/
}
// Recursive Helper Funnction 
template<class Key, class Value>
void AVLTree<Key, Value>::RecursiveInsertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    if (p == nullptr) return;   
    AVLNode<Key, Value>* g = p->getParent();
    if (g == nullptr) return;  
    int diff = 0; 
    if (g->getLeft() == p) diff = -1; // is left child
    else diff = 1; // is right child 

    g->updateBalance(diff);
    int grandbalance = g->getBalance();

    if (grandbalance == 0) return;
    if (grandbalance == 1 || grandbalance == -1) RecursiveInsertFix(g, p);
    if (grandbalance == -2 || grandbalance == 2)
		{
			if (grandbalance < 0 && p->getBalance() < 0) //Left Zig Zig
			{
				RotateRight(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else if (grandbalance < 0 && p->getBalance() > 0) //Left Zig Zag
			{
				RotateLeft(p);
				RotateRight(g);
				//set balance 
				if (n->getBalance() == -1)
				{
					p->setBalance(0); g->setBalance(1); n->setBalance(0);
				}
				else if (n->getBalance() == 0)
				{
					p->setBalance(0); g->setBalance(0); n->setBalance(0);
				}
				else if (n->getBalance() == 1)
				{
					p->setBalance(-1); g->setBalance(0); n->setBalance(0);
				}
			}
			else if (grandbalance > 0 && p->getBalance() > 0) //Right Zig Zig
			{
				RotateLeft(g);
				p->setBalance(0);
				g->setBalance(0);
			}
			else if (grandbalance > 0 && p->getBalance() < 0) //Right ZigZag
			{
				RotateRight(p);
				RotateLeft(g);
				//set balance 
				if (n->getBalance() == -1)
				{
					p->setBalance(1); g->setBalance(0); n->setBalance(0);
				}
				else if (n->getBalance() == 0)
				{
					p->setBalance(0); g->setBalance(0); n->setBalance(0);
				}
				else if (n->getBalance() == 1)
				{
					p->setBalance(0); g->setBalance(-1); n->setBalance(0);
				}
			}
		}
 
}

// Rotation Helper Function

template<class Key, class Value>
void AVLTree<Key, Value>::RotateRight(AVLNode<Key, Value>* n)
{
	AVLNode<Key, Value>* leftchild = n->getLeft();
	if (leftchild==nullptr) return;
	AVLNode<Key, Value>* parent = n->getParent();
	if (parent != nullptr)
	{
		if (parent -> getLeft() == n) parent->setLeft(leftchild);
		else parent->setRight(leftchild);
	}
	else
	{
		this->root_ = leftchild;
	}
	AVLNode<Key, Value>* childRight = leftchild->getRight();
	n->setLeft(childRight);
	leftchild->setRight(n);
	leftchild->setParent(parent);
	n->setParent(leftchild);
	if (childRight != nullptr) childRight->setParent(n);

	return;
}
template<class Key, class Value>
void AVLTree<Key, Value>::RotateLeft(AVLNode<Key, Value>* n)
{
	AVLNode<Key, Value>* rightchild = n->getRight();
	if (rightchild==nullptr) return;
	AVLNode<Key, Value>* parent = n->getParent();
	if (parent != nullptr)
	{
		if (parent -> getLeft() == n) parent->setLeft(rightchild);
		else parent->setRight(rightchild);
	}
	else
	{
		this->root_ = rightchild;
	}
	AVLNode<Key, Value>* childLeft = rightchild->getLeft();
	n->setRight(childLeft);
	rightchild->setLeft(n);
	rightchild->setParent(parent);
	n->setParent(rightchild);
	if (childLeft != nullptr) childLeft->setParent(n);

	return;
}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
	AVLNode<Key,Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
	if (n == nullptr) return;
	int diff = 0;
	
  AVLNode<Key,Value>* p = n->getParent();
	if (p == nullptr) 
	{
		//remove single root 
		if (n->getLeft() == nullptr)
		{
			if (n->getRight() == nullptr)
			{
				delete n;
				this->root_ = nullptr;
				return;
			}
			else // no predecessor
			{
				AVLNode<Key,Value>* c = n->getRight();
				c->setParent(nullptr);
				delete n;
				this->root_ = c;
				return;
			}
		}
		AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->predecessor(this->root_));
		this->nodeSwap(n, temp);
		p = n->getParent();
		if (p != nullptr)	
		{
			if (p->getLeft() == n) diff = 1; //leftchild
			else diff = -1;
		}
		if (n == this->root_) this->root_ = temp;
        //no child
		if (n->getLeft() == nullptr && n->getRight() == nullptr)
		{
            Node<Key, Value> * tempparent = n->getParent();
            if (tempparent->getLeft() == n)tempparent->setLeft(nullptr);
            else tempparent->setRight(nullptr);
			delete n;
		}
        
		//One Child
		else if (n->getLeft() != nullptr)
		{
			Node<Key, Value> * tempself = n->getLeft();
			Node<Key, Value> * tempparent = n->getParent();
            if (tempparent->getLeft() == n)tempparent->setLeft(tempself);
            else tempparent->setRight(tempself);
			delete n; 
			tempself->setParent(tempparent);
		}
	}
	else 
	{
		if (p != nullptr)	
		{
			if (p->getLeft() == n) diff = 1; //leftchild
			else diff = -1;
		}
		BinarySearchTree<Key, Value>::remove(key);
	}


	//int prevB = n->getBalance();
	

	RecursiveRemoveFix(p,diff);
	/*
	if (p == nullptr) //delete root
	{
		AVLNode<Key,Value>* g = static_cast<AVLNode<Key, Value>*>(this->root_);
		if (g == nullptr) return;
		if (g->getLeft() != nullptr && g->getRight() != nullptr) // two child 
		{
			if (prevB == -1) 
			{
				g->setBalance(0);
				return;
			}
			if (prevB == 1)
			{
				if (g->getRight()->getRight() != nullptr)	//ZigZig
				{
					AVLNode<Key,Value>* c = g->getRight();
					this->RotateLeft(g);
					g->setBalance(0);
					c->setBalance(0);
				}
				else 
				{
					AVLNode<Key,Value>* c = g->getRight();
					AVLNode<Key,Value>* gc = c->getLeft();
					this->RotateRight(c);
					this->RotateLeft(g);
					if (gc->getBalance() == -1)
					{
						c->setBalance(1); g->setBalance(0); gc->setBalance(0);
					}
					else if (gc->getBalance() == 0)
					{
						c->setBalance(0); g->setBalance(0); gc->setBalance(0);
					}
					else if (gc->getBalance() == 1)
					{
						c->setBalance(0); g->setBalance(-1); gc->setBalance(0);
					}
				}
				return;
			}
			else 
			{
				g->setBalance(-1);
			}
		}
	}
	*/
}

template<class Key, class Value>
void AVLTree<Key, Value>::RecursiveRemoveFix ( AVLNode<Key,Value>* n, int diff)
{
	if (n == nullptr) return;
	int nextdiff = 0;
	AVLNode<Key,Value>* p = n->getParent();
	if(p!=nullptr) 
	{
		if(p->getLeft() == n) nextdiff = 1;
		else nextdiff = -1;
	}
	if (n->getBalance() + diff == -2) //left
	{
		AVLNode<Key,Value>* c = n->getLeft();
		if (c->getBalance() == -1) //zig zig
		{
			RotateRight(n);
			n->setBalance(0);
			c->setBalance(0);
			RecursiveRemoveFix(p, nextdiff);
			return;
		}
		else if (c->getBalance() == 0) //zig zig
		{
			RotateRight(n);
			n->setBalance(-1);
			c->setBalance(1);
			return;
		}
		else if (c->getBalance() == 1) //zig zag
		{
			AVLNode<Key,Value>* g = c->getRight();
			RotateLeft(c);
			RotateRight(n);
			if(g->getBalance() == 1)
			{
				n->setBalance(0);
				c->setBalance(-1);
				g->setBalance(0);
			}
			else if(g->getBalance() == 0)
			{
				n->setBalance(0);
				c->setBalance(0);
				g->setBalance(0);
			}
			else if(g->getBalance() == -1)
			{
				n->setBalance(1);
				c->setBalance(0);
				g->setBalance(0);
			}
			RecursiveRemoveFix(p, nextdiff);
			return;
		}
	}
	else if (n->getBalance() + diff == 2) //right 
	{
		AVLNode<Key,Value>* c = n->getRight();
		if (c->getBalance() == 1) //zig zig
		{
			RotateLeft(n);
			n->setBalance(0);
			c->setBalance(0);
			RecursiveRemoveFix(p, nextdiff);
			return;
		}
		else if (c->getBalance() == 0) //zig zig
		{
			RotateLeft(n);
			n->setBalance(1);
			c->setBalance(-1);
			return;
		}
		else if (c->getBalance() == -1) //zig zag
		{
			AVLNode<Key,Value>* g = c->getLeft();
			RotateRight(c);
			RotateLeft(n);
			if(g->getBalance() == 1)
			{
				n->setBalance(-1);
				c->setBalance(0);
				g->setBalance(0);
			}
			else if(g->getBalance() == 0)
			{
				n->setBalance(0);
				c->setBalance(0);
				g->setBalance(0);
			}
			else if(g->getBalance() == -1)
			{
				n->setBalance(0);
				c->setBalance(1);
				g->setBalance(0);
			}
			RecursiveRemoveFix(p, nextdiff);
			return;
		}
	}
	else if (n->getBalance() + diff == -1 || n->getBalance() + diff == 1)
	{
		n->setBalance(n->getBalance() + diff);
		return;
	}
	else //n->getBalance() + diff == 0
	{
		n->setBalance(0);
		RecursiveRemoveFix(p, nextdiff);
		return;
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

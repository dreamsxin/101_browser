#ifndef _BinaryTreeHelper_h
#define _BinaryTreeHelper_h

#include <cassert>
#include <algorithm>

#include "BasicDataStructures/Tree/BinaryTree.h"
#include "BasicDataStructures/Permuter.h"

template <typename Type, typename NodePropertyType> bool isRoot(
	BinaryTreeNode<Type, NodePropertyType>* in_pNode)
{
	assert(in_pNode);

	return in_pNode->parent() == NULL;
}

template <typename Type, typename NodePropertyType> bool isLeftChild(
	BinaryTreeNode<Type, NodePropertyType>* in_pNode)
{
	assert(in_pNode);
	assert(!isRoot(in_pNode));

	return in_pNode->parent()->left() == in_pNode;
}

// If this function does not return NULL, the return value
// is the new root of the tree
template <typename Type, typename NodePropertyType> BinaryTreeNode<Type, NodePropertyType>* rotateRight(
	BinaryTreeNode<Type, NodePropertyType>* in_pNode)
{	
	BinaryTreeNode<Type, NodePropertyType>* pNode3 = in_pNode;
	assert(pNode3 != NULL);

	if (pNode3 == NULL)
		return NULL;

	BinaryTreeNode<Type, NodePropertyType>* pNode1=in_pNode->left();
	assert(pNode1 != NULL);

	if (pNode1 == NULL)
		return NULL;

	/*
	Original:
	           R* (R as root - not necessarily root of tree)
			  /
		     3 <- in_pNode (can also be right child of root)
		    / \
		   1   4*
	      / \
	     0*  2*

	Result:
	           R* (R as root - not necessarily root of tree)
			  /
		     1
		    / \
		   0*  3
	          / \
	         2*  4*

	     * does not necessarily exist

	*/

	BinaryTreeNode<Type, NodePropertyType>** positions[]=
	{
		&pNode1->right(),
		&pNode3->left(),
		&pNode3->parent(),
		&pNode1->parent()
	};

	permuteForward(positions, 4);

	// it is already rotated, so pNode1->right() changes to pNode3->left()
	// the single = is correct
	if (BinaryTreeNode<Type, NodePropertyType>* pNode2 = pNode3->left())
	{
		pNode2->parent()=pNode3;
	}

	// it is already rotated, so pNode3->parent() changes to pNode1->parent()
	// the single = is correct
	if (BinaryTreeNode<Type, NodePropertyType>* pNodeRoot = pNode1->parent())
	{
		if (pNodeRoot->left() == pNode3)
		{
			pNodeRoot->left() = pNode1;
		}
		else
		{
			assert(pNodeRoot->right() == pNode3);
			pNodeRoot->right() = pNode1;
		}
	}
	else
	{
		return pNode1;
	}

	return NULL;
}

// If this function does not return NULL, the return value
// is the new root of the tree
template <typename Type, typename NodePropertyType> BinaryTreeNode<Type, NodePropertyType>* rotateLeft(
	BinaryTreeNode<Type, NodePropertyType>* in_pNode)
{
	BinaryTreeNode<Type, NodePropertyType>* pNode1 = in_pNode;
	assert(pNode1 != NULL);

	if (pNode1 == NULL)
		return NULL;

	BinaryTreeNode<Type, NodePropertyType>* pNode3 = in_pNode->right();
	assert(pNode3 != NULL);
	
	if (pNode3 == NULL)
		return NULL;

	/*
	Original:
	           R* (R as root - not necessarily root of tree)
			  /
		     1 <- in_pNode (can also be right child of root)
		    / \
		   0*  3
	          / \
	         2*  4*
	
	Result:
	           R* (R as root - not necessarily root of tree)
			  /
		     3
		    / \
		   1   4*
	      / \
	     0*  2*

	     * does not necessarily exist

	*/

	BinaryTreeNode<Type, NodePropertyType>** positions[]=
	{
		&pNode1->right(),
		&pNode3->left(),
		&pNode3->parent(),
		&pNode1->parent()
	};

	permuteBackward(positions, 4);

	// it is already rotated, so pNode3->left() changes to pNode1->right()
	// the single = is correct
	if (BinaryTreeNode<Type, NodePropertyType>* pNode2=pNode1->right())
	{
		pNode2->parent() = pNode1;
	}

	// it is already rotated, so pNode1->parent() changes to pNode3->parent()
	// the single = is correct
	if (BinaryTreeNode<Type, NodePropertyType>* pNodeRoot = pNode3->parent())
	{
		if (pNodeRoot->left() == pNode1)
		{
			pNodeRoot->left() = pNode3;
		}
		else
		{
			assert(pNodeRoot->right() == pNode1);
			pNodeRoot->right() = pNode3;
		}
	}
	else
	{
		return pNode3;
	}

	return NULL;
}

#endif

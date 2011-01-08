/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BinaryTreeHelper_h
#define _BinaryTreeHelper_h

#include <cassert>
#include <algorithm>

#include "BasicDataStructures/Tree/BinaryTree.h"
#include "BasicDataStructures/Permuter.h"

enum NodePointerType
{
	NodePointerTypeNULL,
	NodePointerTypeRoot,
	NodePointerTypeLeftChild,
	NodePointerTypeRightChild
};

template <typename Type, typename NodePropertyType> NodePointerType getNodePointerType(
	BinaryTreeNode<Type, NodePropertyType>* in_pNode)
{
	if (NULL == in_pNode)
		return NodePointerTypeNULL;
	else if (NULL == in_pNode->parent())
		return NodePointerTypeRoot;
	else if (in_pNode->parent()->left() == in_pNode)
		return NodePointerTypeLeftChild;
	else
	{
		assert(in_pNode->parent()->right() == in_pNode);
		return NodePointerTypeRightChild;
	}
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

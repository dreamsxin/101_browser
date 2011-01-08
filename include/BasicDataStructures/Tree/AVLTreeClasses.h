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

#ifndef _AVLTreeClasses_h
#define _AVLTreeClasses_h

#include <cassert>

#include "BasicDataStructures/Tree/BinaryTree.h"
#include "BasicDataStructures/Tree/BinaryTreeHelper.h"

typedef signed char AVLTreeNodePropertyType;

// If this function does not return NULL, the return value
// is the new root of the tree
template <typename Type> BinaryTreeNode<Type, AVLTreeNodePropertyType>* avlTreeInserted(
	BinaryTreeNode<Type, AVLTreeNodePropertyType>* in_pNode)
{
	if (in_pNode == NULL)
		return NULL;

	in_pNode->nodeProperty() = 0;

	BinaryTreeNode<Type, AVLTreeNodePropertyType>* pActNode = in_pNode;

	while (getNodePointerType(pActNode) != NodePointerTypeRoot)
	{
		assert((pActNode->parent()->nodeProperty() == -1) || (pActNode->parent()->nodeProperty() == 0) || 
			(pActNode->parent()->nodeProperty() == 1));
		
		if (getNodePointerType(pActNode) == NodePointerTypeLeftChild)
		{
			pActNode->parent()->nodeProperty()--;
		}
		else
		{
			assert(getNodePointerType(pActNode) == NodePointerTypeRightChild);
			pActNode->parent()->nodeProperty()++;
		}

		pActNode=pActNode->parent();

		if (pActNode->nodeProperty() == 0)
			return NULL;
		// if it is -1 or 1 -> simply continue
		else if (pActNode->nodeProperty() == -2)
		{
			BinaryTreeNode<Type, AVLTreeNodePropertyType>* pChild = pActNode->left();
			assert(pChild);

			if (pChild == NULL)
				return NULL;

			assert(pChild->nodeProperty() == -1 || pChild->nodeProperty() == 1);

			if (pChild->nodeProperty() == -1)
			{
				return rotateAVLTreeRight(pActNode);
			}
			else 
			{
				assert(pChild->nodeProperty() == 1);

				BinaryTreeNode<Type, AVLTreeNodePropertyType>* res = rotateAVLTreeLeft(pChild);

				// pChild is not root; so rotateAVLTreeLeft must not set a new root
				// (=return of a non-NULL value)
				assert(!res);
				return rotateAVLTreeRight(pActNode);
			}
			break;
		}
		else if (pActNode->nodeProperty() == 2)
		{
			BinaryTreeNode<Type, AVLTreeNodePropertyType>* pChild = pActNode->right();
			assert(pChild);

			if (pChild == NULL)
				return NULL;

			assert(pChild->nodeProperty() == -1 || pChild->nodeProperty() == 1);

			if (pChild->nodeProperty() == 1)
			{
				return rotateAVLTreeLeft(pActNode);
			}
			else
			{
				assert(pChild->nodeProperty() == -1);

				BinaryTreeNode<Type, AVLTreeNodePropertyType>* res = rotateAVLTreeRight(pChild);

				// pChild is not root; so rotateAVLTreeRight must not set a new root
				// (=return of a non-NULL value)
				assert(!res);
				return rotateAVLTreeLeft(pActNode);
			}
			break;
		}
	}

	return NULL;
}

template <typename Type> struct AVLTreeDefiner
{
	typedef BinaryTree<Type, AVLTreeNodePropertyType> AVLTree;
};

#endif

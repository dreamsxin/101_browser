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

#ifndef _BinaryTreeMethods_h
#define _BinaryTreeMethods_h

#include <queue>
#include "Template/Memory/SafeMemoryManagement.h"
#include "BasicDataStructures/Tree/BinaryTreeClasses.h"

template <typename Type, typename NodePropertyType> void deleteBinaryTreeNodeTree(
	BinaryTreeNode<Type, NodePropertyType>* in_pRoot)
{
	std::queue<BinaryTreeNode<Type, NodePropertyType>*> nodeQueue;
	nodeQueue.push(in_pRoot);

	while (!nodeQueue.empty())
	{
		BinaryTreeNode<Type, NodePropertyType>* actNode = nodeQueue.front();

		nodeQueue.pop();

		if (actNode->left())
			nodeQueue.push(actNode->left());
		if (actNode->right())
			nodeQueue.push(actNode->right());

		safe_delete(&actNode);
	}
}

template <typename Type, typename NodePropertyType> InsertResult BinaryTree<Type, NodePropertyType>::insert(
	const Type& in_datum)
{
	BinaryTreeNode<Type, NodePropertyType>* actNode=root();

	if (!root())
	{
		root() = new BinaryTreeNode<Type, NodePropertyType>(in_datum, NULL);
		(*mpInserted)(root());

		InsertResult out_insertResult;
		out_insertResult.ok = true;
		return out_insertResult;
	}

	while (true)
	{
		ExtendedCompareResult res=(*mpCompare)(in_datum, actNode->datum());

		if (res == ExtendedCompareResultLess)
		{
			if (actNode->left() != NULL)
				actNode=actNode->left();
			else
			{
				actNode->left()=new BinaryTreeNode<Type, NodePropertyType>(in_datum, actNode);
				BinaryTreeNode<Type, NodePropertyType>* result=(*mpInserted)(actNode->left());

				// This means we have a new root
				if (result)
				{
					root()=result;
				}

				InsertResult out_insertResult;
				out_insertResult.ok = true;
				return out_insertResult;
			}
		}
		else if (res == ExtendedCompareResultGreater)
		{
			if (actNode->right()!=NULL)
				actNode=actNode->right();
			else
			{
				actNode->right()=new BinaryTreeNode<Type, NodePropertyType>(in_datum, actNode);
				BinaryTreeNode<Type, NodePropertyType>* result=(*mpInserted)(actNode->right());

				// This means we have a new root
				if (result)
				{
					root()=result;
				}

				InsertResult out_insertResult;
				out_insertResult.ok = true;
				return out_insertResult;
			}
		}
		else
		{
			InsertResult out_insertResult = {false, res};
			return out_insertResult;
		}
	}
}

template <typename Type, typename NodePropertyType> template <typename OtherType>
const Type* BinaryTree<Type, NodePropertyType>::get(
	const OtherType& in_pItem, 
	ExtendedCompareResult (*in_pContainedIn)(const OtherType&, const Type&))
{
	BinaryTreeNode<Type, NodePropertyType>* actNode=root();

	Type* out_getResult = NULL;

	if (actNode==NULL)
		return NULL;

	bool terminate = false;

	while (!terminate)
	{
		ExtendedCompareResult compResult=(*in_pContainedIn)(in_pItem, actNode->datum());

		switch (compResult)
		{
		case ExtendedCompareResultEqual:
		case ExtendedCompareResultContainedIn:
			out_getResult = actNode->datum();
			// the missing break ist correct
		case ExtendedCompareResultContains:
		case ExtendedCompareResultNotComparableLess:
		case ExtendedCompareResultNotComparableGreater:
		case ExtendedCompareResultErrorInCode:
			terminate = true;
			break;
		case ExtendedCompareResultLess:
			actNode = actNode->left();
			break;
		case ExtendedCompareResultGreater:
			actNode = actNode->right();
			break;
		}
	}

	return out_getResult;
}

#endif

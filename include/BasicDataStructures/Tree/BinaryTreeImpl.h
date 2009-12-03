#ifndef _BinaryTreeMethods_h
#define _BinaryTreeMethods_h

#include <queue>
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

		delete actNode;
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
		CompareResult res=(*mpCompare)(in_datum, actNode->datum());

		if (res==Less)
		{
			if (actNode->left()!=NULL)
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
		else if (res==Greater)
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
	CompareResult (*in_pContainedIn)(const OtherType&, const Type&))
{
	BinaryTreeNode<Type, NodePropertyType>* actNode=root();

	Type* out_result = NULL;

	if (actNode==NULL)
		return NULL;

	terminate = false;

	while (!terminate)
	{
		CompareResult compResult=(*in_pContainedIn)(in_pItem, actNode->datum());

		switch (compResult)
		{
		case Equal:
		case ContainedIn:
			out_result = actNode->datum();
			// the missing break ist correct
		case Contains:
		case NotComparableLess:
		case NotComparableGreater:
		case ErrorInCode:
			terminate = true;
			break;
		case Less:
			actNode = actNode->left();
			break;
		case Greater:
			actNode = actNode->right();
			break;
		}
	}

	return out_findResult;
}

#endif

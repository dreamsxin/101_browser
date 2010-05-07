#ifndef _BooleanTree_h
#define _BooleanTree_h

#include "BooleanStructure.h"

class TreeNode : public BooleanStructure<const std::vector<bool>*>
{
public:
	bool isLeaf;

	TreeNode(bool in_isLeaf) : isLeaf(in_isLeaf) { }
	virtual ~TreeNode() { }
	virtual size_t size() = 0;
};

class LeafTreeNode : public TreeNode
{
public:
	size_t variableNumber;

	LeafTreeNode() : TreeNode(true), variableNumber(0) { }

	virtual bool computeValue(const std::vector<bool>* in_pVariables);
	virtual void print();
	virtual void reset();
	virtual bool increment();
	virtual size_t size();
};

class FunctionTreeNode 
	: public TreeNode, public FunctionBooleanStructure<const std::vector<bool>*>
{
public:
	TreeNode *child0, *child1;

	FunctionTreeNode(FunctionType in_functionType, size_t in_childrenCount) 
		: TreeNode(false), 
		FunctionBooleanStructure<const std::vector<bool>*>(in_functionType),
		child0(NULL), child1(NULL)
	{
		if (in_childrenCount==0)
		{
			child0 = new LeafTreeNode();
		}
		else
		{
			child0 = new FunctionTreeNode(FunctionTypeNot, in_childrenCount-1);
		}

		if (functionType != FunctionTypeNot)
		{
			child1 = new LeafTreeNode();
		}
	}

	virtual ~FunctionTreeNode()
	{
		delete child0;
		child0 = NULL;
		if (child1 != NULL)
			delete child1;
		child1 = NULL;
	}

	virtual bool computeValue(const std::vector<bool>* in_pVariables);
	virtual void print();
	virtual void reset();

	/*!
	 * This function must only be called if the node *is*
	 * the root node
	 */
	inline virtual bool rootIncrement()
	{
		return increment(true);
	}

	/*!
	 * This function must only be called if the node is *not*
	 * the root node
	 */
	inline virtual bool increment()
	{
		return increment(false);
	}

	virtual bool increment(bool isRoot);
	virtual size_t size();
};

#endif

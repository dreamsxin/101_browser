#ifndef _BooleanTree_h
#define _BooleanTree_h

#include "BooleanStructure.h"

class TreeNode : public BooleanStructure<bool, void*>
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

	virtual bool computeValue(const std::vector<bool>* variables, void*);
	virtual void print();
	virtual void reset();
	virtual bool increment(bool isRoot);
	virtual size_t size();
};

class FunctionTreeNode : public TreeNode, public FunctionBooleanStructure<bool, void*>
{
public:
	TreeNode *child0, *child1;

	FunctionTreeNode(FunctionType in_functionType, size_t in_childrenCount) 
		: TreeNode(false), 
		FunctionBooleanStructure(in_functionType),
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

	virtual bool computeValue(const std::vector<bool>* variables, void*);
	virtual void print();
	virtual void reset();
	virtual bool increment(bool isRoot);
	virtual size_t size();
};

#endif

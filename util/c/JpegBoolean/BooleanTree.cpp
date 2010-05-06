#include "BooleanTree.h"
#include <cassert>

bool LeafTreeNode::computeValue(const std::vector<bool>* in_pVariables)
{
	return in_pVariables->at(variableNumber);
}

void LeafTreeNode::print()
{
	printf("x%u", variableNumber);
}

void LeafTreeNode::reset()
{
	variableNumber = 0;
}

bool LeafTreeNode::increment(bool isRoot)
{
	extern size_t varCount;

	if (variableNumber+1<varCount)
	{
		variableNumber++;
		return true;
	}
	else
		return false;
}

size_t LeafTreeNode::size()
{
	return 0;
}

bool FunctionTreeNode::computeValue(const std::vector<bool>* in_pVariables)
{
	switch (functionType)
	{
	case FunctionTypeNot:
		return !child0->computeValue(in_pVariables);
	case FunctionTypeXor:
		return child0->computeValue(in_pVariables) ^ child1->computeValue(in_pVariables);
	case FunctionTypeAnd:
		return child0->computeValue(in_pVariables) && child1->computeValue(in_pVariables);
	case FunctionTypeOr:
		return child0->computeValue(in_pVariables) || child1->computeValue(in_pVariables);
	}

	// should never happen
	return false;
}

void FunctionTreeNode::print()
{
	if (functionType == FunctionTypeNot) {
		printf("!");
		bool parantheses = true;
		if (child0->isLeaf)
			parantheses = false;
		else if (((FunctionTreeNode*) child0)->functionType == FunctionTypeNot)
			parantheses = false;

		if (parantheses)
			printf("(");
		child0->print();
		if (parantheses)
			printf(")");
	}
	else
	{
		bool parantheses;

		parantheses = true;
		if (child0->isLeaf)
			parantheses = false;
		else if (((FunctionTreeNode*) child0)->functionType == FunctionTypeNot)
			parantheses = false;

		if (parantheses)
			printf("(");
		child0->print();
		if (parantheses)
			printf(")");

		switch (functionType)
		{
		case FunctionTypeXor:
			printf(" ^ ");
			break;
		case FunctionTypeAnd:
			printf(" && ");
			break;
		case FunctionTypeOr:
			printf(" || ");
			break;
		}

		parantheses = true;
		if (child1->isLeaf)
			parantheses = false;
		else if (((FunctionTreeNode*) child1)->functionType == FunctionTypeNot)
			parantheses = false;

		if (parantheses)
			printf("(");
		child1->print();
		if (parantheses)
			printf(")");
	}
}

void FunctionTreeNode::reset()
{
	int oldSize = size();

	functionType = FunctionTypeNot;
	assert(child0 != NULL);
	delete child0;
	child0 = NULL;

	if (oldSize == 1) {
		child0 = new LeafTreeNode();
	} else {
		child0 = new FunctionTreeNode(FunctionTypeNot, oldSize-2);
	}

	if (child1 != NULL)
	{
		delete child1;
		child1 = NULL;
	}

	assert(child1 == NULL);
}

bool FunctionTreeNode::increment(bool isRoot)
{
	assert(child0 != NULL);
	if (child0->increment(false))
		return true;
	if (child1 != NULL && child1->increment(false))
	{
		child0->reset();
		return true;
	}
	if (functionType != FunctionTypeOr && !isRoot)
	{
		FunctionType funcTypeBak = functionType;
		reset();
		assert(child1 == NULL);
		child1 = new LeafTreeNode();

		if (funcTypeBak == FunctionTypeNot)
		{
			functionType = FunctionTypeXor;
		}
		else if (funcTypeBak == FunctionTypeXor)
		{
			functionType = FunctionTypeAnd;
		}
		else if (funcTypeBak == FunctionTypeAnd)
		{
			functionType = FunctionTypeOr;
		}

		return true;
	}
	if (child0->size() != 0 && (!isRoot || (isRoot && functionType != FunctionTypeNot)))
	{
		size_t child0size = child0->size();
		assert(functionType != FunctionTypeNot);
		assert(child1 != NULL);
		size_t child1size = child1->size();

		delete child0;
		child0 = NULL;

		if (child0size == 1)
		{
			child0 = new LeafTreeNode();
		}
		else
		{
			child0 = new FunctionTreeNode(FunctionTypeNot, child0size-2);
		}

		assert(child1 != NULL);
		delete child1;
		child1 = new FunctionTreeNode(FunctionTypeNot, child1size);

		return true;
	}

	return false;
}

size_t FunctionTreeNode::size()
{
	assert(child0 != NULL);
	size_t out_size = 1+child0->size();

	if (functionType != FunctionTypeNot)
	{
		assert(child1 != NULL);
		out_size += child1->size();
	}

	return out_size;
}

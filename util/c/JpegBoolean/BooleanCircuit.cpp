#include "BooleanCircuit.h"
#include <cstdio>

BooleanCircuitNode::BooleanCircuitNode(FunctionType in_functionType) 
: FunctionBooleanStructure<const std::vector<bool>*>(in_functionType)
{
	child0Index = 0;

	if (in_functionType == FunctionTypeNot)
		child1Index = 0;
	else
		child1Index = 1;
}

bool BooleanCircuitNode::computeValue(const std::vector<bool>* in_pValues)
{
	switch (functionType)
	{
	case FunctionTypeNot:
		return !in_pValues->at(child0Index);
	case FunctionTypeXor:
		return in_pValues->at(child0Index) ^ in_pValues->at(child1Index);
	case FunctionTypeAnd:
		return in_pValues->at(child0Index) && in_pValues->at(child1Index);
	case FunctionTypeOr:
		return in_pValues->at(child0Index) || in_pValues->at(child1Index);
	}

	// should never happen
	return false;
}

void BooleanCircuitNode::print()
{
	switch (functionType)
	{
	case FunctionTypeNot:
		printf("Not %u\n", child0Index);
		break;
	case FunctionTypeXor:
		printf("Xor %u %u\n", child0Index, child1Index);
		break;
	case FunctionTypeAnd:
		printf("And %u %u\n", child0Index, child1Index);
		break;
	case FunctionTypeOr:
		printf("Or %u %u\n", child0Index, child1Index);
		break;
	}
}

void BooleanCircuitNode::reset()
{
	functionType = FunctionTypeNot;
	child0Index = 0;
	child1Index = 0; // Not necessary, but does not harm
}

bool BooleanCircuitNode::increment(BooleanCircuitNodeIncrementParameters in_parameters)
{
	extern size_t varCount;
	size_t maxChildIndex = varCount+in_parameters.thisNodeIndex-1;

	if (child0Index < maxChildIndex && functionType == FunctionTypeNot)
	{
		child0Index++;
		return true;
	}
	else if (child0Index < maxChildIndex && functionType != FunctionTypeNot 
		&& child0Index+1 < child1Index)
	{
		child0Index++;
		return true;
	}
	else if (functionType != FunctionTypeNot && child1Index < maxChildIndex)
	{
		child0Index = 0;
		child1Index++;
		return true;
	}
	else if (functionType != FunctionTypeOr && !in_parameters.isFinalNode)
	{
		if (functionType == FunctionTypeNot)
		{
			functionType = FunctionTypeXor;
		}
		else if (functionType == FunctionTypeXor)
		{
			functionType = FunctionTypeAnd;
		}
		else if (functionType == FunctionTypeAnd)
		{
			functionType = FunctionTypeOr;
		}

		child0Index = 0;
		child1Index = 1;

		return true;
	}

	return false;
}


BooleanCircuitNetwork::BooleanCircuitNetwork(FunctionType in_functionTypeAtLast, size_t in_nodeCount) 
: FunctionBooleanStructure<const std::vector<bool>*>(in_functionTypeAtLast)
{
	extern size_t varCount;

	nodes.reserve(in_nodeCount);
	values.reserve(varCount+in_nodeCount);

	for (size_t i = 0; i<in_nodeCount-1; i++)
	{
		nodes.push_back(BooleanCircuitNode(FunctionTypeNot));
	}
	nodes.push_back(BooleanCircuitNode(in_functionTypeAtLast));

	for (size_t i=0; i<varCount+in_nodeCount; i++)
	{
		values.push_back(false);
	}
}

bool BooleanCircuitNetwork::computeValue(const std::vector<bool>* variables)
{
	extern size_t varCount;

	for (size_t i=0; i<varCount; i++)
	{
		values[i] = (*variables)[i];
	}

	for (size_t i=0; i<nodes.size(); i++)
	{
		values[varCount+i] = nodes[i].computeValue(&values);
	}

	return values[varCount+nodes.size()-1];
}

void BooleanCircuitNetwork::print()
{
	extern size_t varCount;

	for (size_t i=0; i<varCount; i++)
	{
		printf("%u: x%u\n", i, i);
	}

	for (size_t i=0; i<nodes.size(); i++)
	{
		printf("%u: ", i+varCount, i);
		nodes.at(i).print();
	}
}

bool BooleanCircuitNetwork::increment()
{
	size_t nodesSize = nodes.size();

	for (size_t currentIndexPlusOne = nodesSize; currentIndexPlusOne > 0; currentIndexPlusOne--)
	{
		BooleanCircuitNodeIncrementParameters p;
		p.thisNodeIndex = currentIndexPlusOne-1;
		p.isFinalNode = (currentIndexPlusOne == nodesSize);

		if (nodes[currentIndexPlusOne-1].increment(p))
		{
			if (currentIndexPlusOne<nodesSize)
			{
				nodes[nodesSize-1].child0Index = 0;
				nodes[nodesSize-1].child1Index = 0;

				for (size_t currentIndexPlusOne2 = nodesSize-1; 
					currentIndexPlusOne2 > currentIndexPlusOne; currentIndexPlusOne2--)
				{
					nodes[currentIndexPlusOne2-1].reset();
				}
			}

			return true;
		}
	}

	return false;
}

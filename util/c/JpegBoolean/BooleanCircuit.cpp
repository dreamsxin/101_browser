#include "BooleanCircuit.h"
#include <cstdio>

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
	
	if (child0Index < maxChildIndex)
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
		child1Index = 0;

		return true;
	}

	return false;
}



bool BooleanCircuitNetwork::computeValue(const std::vector<bool>* variables)
{
	extern size_t varCount;

	for (size_t i=0; i<varCount; i++)
	{
		values.at(i) = variables->at(i);
	}

	for (size_t i=0; i<nodes.size(); i++)
	{
		values.at(varCount+i) = nodes.at(i).computeValue(&values);
	}

	return values.at(varCount+nodes.size()-1);
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
	size_t currentIndex;

	for (currentIndex = 0; currentIndex < nodes.size(); currentIndex++)
	{
		BooleanCircuitNodeIncrementParameters p;
		p.thisNodeIndex = currentIndex;
		p.isFinalNode = (currentIndex == (nodes.size()-1));

		if (nodes.at(currentIndex).increment(p))
		{
			for (size_t currentIndex2 = 0; currentIndex2 < currentIndex; currentIndex2++)
			{
				nodes.at(currentIndex2).reset();
			}

			return true;
		}
	}

	return false;
}

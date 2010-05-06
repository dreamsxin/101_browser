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

bool BooleanCircuitNode::increment(BooleanCircuitNodeIncrementPair in_incrementPair)
{
	return true;
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

bool BooleanCircuitNetwork::increment(void*)
{
	return true;
}

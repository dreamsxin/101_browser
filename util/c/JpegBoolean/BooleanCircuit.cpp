#include "BooleanCircuit.h"

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

}

void BooleanCircuitNode::reset()
{

}

bool BooleanCircuitNode::increment(size_t in_thisNodeIndex)
{
	return true;
}



bool BooleanCircuitNetwork::computeValue(const std::vector<bool>* variables, 
										 void*)
{
	return true;
}

void BooleanCircuitNetwork::print()
{

}

void BooleanCircuitNetwork::reset()
{

}

bool BooleanCircuitNetwork::increment(size_t in_nodeCount)
{
	return true;
}

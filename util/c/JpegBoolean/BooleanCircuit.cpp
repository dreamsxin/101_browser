#include "BooleanCircuit.h"

bool BooleanCircuitNode::computeValue(const std::vector<bool>* variables, 
									  const std::vector<BooleanCircuitNode>* in_pNodes)
{
	return true;
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

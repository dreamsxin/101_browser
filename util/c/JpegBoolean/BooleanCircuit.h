#ifndef _BooleanCircuit_h
#define _BooleanCircuit_h

#include "BooleanStructure.h"
#include <vector>

class BooleanCircuitNode : public FunctionBooleanStructure<size_t, 
	const std::vector<bool>*>
{
public:
	size_t child0Index, child1Index;

	BooleanCircuitNode(FunctionType in_functionType) 
		: FunctionBooleanStructure(in_functionType)
	{
		child0Index = 0;
		child1Index = 0;
	}

	virtual bool computeValue(const std::vector<bool>* in_pValues);
	virtual void print();
	virtual void reset();
	virtual bool increment(size_t in_thisNodeIndex);
};

class BooleanCircuitNetwork : public FunctionBooleanStructure<size_t, void*>
{
public:
	size_t nodeCount;
	std::vector<BooleanCircuitNode> nodes;
	std::vector<bool> values;

	BooleanCircuitNetwork(FunctionType in_functionTypeAtLast, size_t in_nodeCount) 
		: FunctionBooleanStructure(in_functionTypeAtLast), nodeCount(in_nodeCount)
	{
		extern size_t varCount;

		nodes.reserve(nodeCount);
		values.reserve(varCount+nodeCount);
		
		for (size_t i = 0; i<nodeCount-1; i++)
		{
			nodes.push_back(BooleanCircuitNode(FunctionTypeNot));
		}

		nodes.push_back(BooleanCircuitNode(in_functionTypeAtLast));
	}

	virtual bool computeValue(const std::vector<bool>* variables, void*);
	virtual void print();
	virtual void reset();
	virtual bool increment(size_t in_nodeCount);
};

#endif

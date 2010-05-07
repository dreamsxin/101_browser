#ifndef _BooleanCircuit_h
#define _BooleanCircuit_h

#include "BooleanStructure.h"
#include <vector>
#include <cassert>

struct BooleanCircuitNodeIncrementParameters
{
	size_t thisNodeIndex;
	bool isFinalNode;
};

class BooleanCircuitNode : public FunctionBooleanStructure<const std::vector<bool>*>
{
public:
	size_t child0Index, child1Index;

	BooleanCircuitNode(FunctionType in_functionType) 
		: FunctionBooleanStructure<const std::vector<bool>*>(in_functionType)
	{
		child0Index = 0;
		child1Index = 0;
	}

	virtual bool computeValue(const std::vector<bool>* in_pValues);
	virtual void print();
	virtual void reset();
	/*!
	 * This function should never be called
	 */
	inline virtual bool increment()
	{
		assert(false);
		return false;
	}
	virtual bool increment(BooleanCircuitNodeIncrementParameters in_parameters);
};

class BooleanCircuitNetwork : public FunctionBooleanStructure<const std::vector<bool>*>
{
public:
	std::vector<BooleanCircuitNode> nodes;
	std::vector<bool> values;

	BooleanCircuitNetwork(FunctionType in_functionTypeAtLast, size_t in_nodeCount) 
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

	virtual bool computeValue(const std::vector<bool>* variables);
	virtual void print();
	/*!
	 * It does nothing. This is how it has to be.
	 */
	inline virtual void reset() { }
	virtual bool increment();
};

#endif

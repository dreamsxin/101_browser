#ifndef _BooleanStructure_h
#define _BooleanStructure_h

#include "BooleanThread.h"
#include <vector>

template <typename IncrementType, typename ComputeType> class BooleanStructure
{
public:
	virtual bool computeValue(const std::vector<bool>* variables, ComputeType computeType) = 0;
	virtual void print() = 0;
	virtual void reset() = 0;
	virtual bool increment(IncrementType incrementType) = 0;
};

template <typename IncrementType, typename ComputeType> class FunctionBooleanStructure 
: public BooleanStructure<IncrementType, ComputeType>
{
public:
	FunctionType functionType;

	FunctionBooleanStructure(FunctionType in_functionType) : functionType(in_functionType)
	{ }
};

#endif

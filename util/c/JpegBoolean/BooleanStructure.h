#ifndef _BooleanStructure_h
#define _BooleanStructure_h

#include "BooleanThread.h"
#include <vector>

template <typename ComputeType> class BooleanStructure
{
public:
	virtual bool computeValue(ComputeType computeType) = 0;
	virtual void print() = 0;
	virtual void reset() = 0;
	virtual bool increment() = 0;
};

template <typename ComputeType> class FunctionBooleanStructure 
: public BooleanStructure<ComputeType>
{
public:
	FunctionType functionType;

	FunctionBooleanStructure(FunctionType in_functionType) : functionType(in_functionType)
	{ }
};

#endif

#ifndef _BooleanStructure_h
#define _BooleanStructure_h

#include "BooleanThread.h"
#include <vector>

class BooleanStructure
{
public:
	virtual bool computeValue(const std::vector<bool>* variables) = 0;
	virtual void print() = 0;
	virtual void reset() = 0;
	virtual bool increment(bool isRoot) = 0;
};

class FunctionBooleanStructure : public BooleanStructure
{
public:
	FunctionType functionType;

	FunctionBooleanStructure(FunctionType in_functionType) : functionType(in_functionType)
	{ }
};

#endif

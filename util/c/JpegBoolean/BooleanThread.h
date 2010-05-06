#ifndef _BooleanFunction_h
#define _BooleanFunction_h

#include <cstdio>

enum FunctionType
{
	FunctionTypeNot,
	FunctionTypeXor,
	FunctionTypeAnd,
	FunctionTypeOr
};

struct ThreadInit
{
	size_t functionNumber;
	FunctionType functionAtEnd;

	inline void print()
	{
		printf("Function: %u\tRoot: ", functionNumber);
		switch (functionAtEnd)
		{
		case FunctionTypeNot:
			printf("Not");
			break;
		case FunctionTypeXor:
			printf("Xor");
			break;
		case FunctionTypeAnd:
			printf("And");
			break;
		case FunctionTypeOr:
			printf("Or");
			break;
		}
	}
};

inline size_t getThreadGroupNumber(ThreadInit threadInit)
{
	return threadInit.functionNumber;
}

inline size_t getThreadNumber(ThreadInit threadInit)
{
	return 4*getThreadGroupNumber(threadInit)+threadInit.functionAtEnd;
}

#endif

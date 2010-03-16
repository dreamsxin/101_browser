#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "pthread.h"
using namespace std;

enum FunctionType
{
	FunctionTypeNot,
	FunctionTypeXor,
	FunctionTypeAnd,
	FunctionTypeOr
};

struct ThreadInit
{
	// tells whether to use indexToRowColValues or rowColToIndexValues
	bool rowColToIndex;
	size_t functionNumber;
	FunctionType functionAtRoot;

	void print()
	{
		printf("%s\t", rowColToIndex ? "RowCol to Index" : "Index to RowCol");
		printf("Function: %u\tRoot: ", functionNumber);
		switch (functionAtRoot)
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
		printf("\n");
	}
};

// number of functions for one of en- or decoding
size_t functionsCount;
// this is the same as functionsCount - but this can't be 
// assumed in general, so it is a separate variable
size_t variablesCount;

class TreeNode
{
public:
	bool isLeaf;

	TreeNode(bool in_isLeaf) : isLeaf(in_isLeaf) { }
	virtual ~TreeNode() { }

	virtual bool computeValue(const vector<bool>* variables) = 0;
	virtual void print() = 0;
	virtual bool increment() = 0;
};

class LeafTreeNode : public TreeNode
{
public:
	size_t variableNumber;

	LeafTreeNode(size_t in_variableNumber) 
		: TreeNode(true), variableNumber(in_variableNumber) { }

	virtual bool computeValue(const vector<bool>* variables)
	{
		return variables->at(variableNumber);
	}

	virtual void print()
	{
		printf("x%u", variableNumber);
	}

	virtual bool increment()
	{
		if (variableNumber+1<variablesCount)
		{
			variableNumber++;
			return true;
		}
		else
			return false;
	}
};

class FunctionTreeNode : public TreeNode
{
public:
	FunctionType functionType;
	TreeNode *child0, *child1;

	FunctionTreeNode(FunctionType in_functionType, size_t in_childrenCount) 
		: TreeNode(false), functionType(in_functionType), child0(NULL), child1(NULL)
	{
		if (in_childrenCount==0)
		{
			child0 = new LeafTreeNode(0);
		}
		else
		{
			child0 = new FunctionTreeNode(FunctionTypeNot, in_childrenCount-1);
		}

		if (functionType != FunctionTypeNot)
		{
			child1 = new LeafTreeNode(0);
		}
	}

	virtual ~FunctionTreeNode()
	{
		delete child0;
		child0 = NULL;
		if (child1 != NULL)
			delete child1;
		child1 = NULL;
	}

	virtual bool computeValue(const vector<bool>* variables)
	{
		switch (functionType)
		{
		case FunctionTypeNot:
			return !child0->computeValue(variables);
		case FunctionTypeXor:
			return child0->computeValue(variables) ^ child1->computeValue(variables);
		case FunctionTypeAnd:
			return child0->computeValue(variables) && child1->computeValue(variables);
		case FunctionTypeOr:
			return child0->computeValue(variables) || child1->computeValue(variables);
		}

		// should never happen
		return false;
	}

	virtual void print()
	{
		if (functionType == FunctionTypeNot) {
			printf("!");
			child0->print();
		}
		else
		{
			bool parantheses;

			parantheses = true;
			if (child0->isLeaf)
				parantheses = false;
			else if (((FunctionTreeNode*) child0)->functionType == FunctionTypeNot)
				parantheses = false;

			if (parantheses)
				printf("(");
			child0->print();
			if (parantheses)
				printf(")");

			switch (functionType)
			{
			case FunctionTypeXor:
				printf(" ^ ");
				break;
			case FunctionTypeAnd:
				printf(" && ");
				break;
			case FunctionTypeOr:
				printf(" || ");
				break;
			}

			parantheses = true;
			if (child1->isLeaf)
				parantheses = false;
			else if (((FunctionTreeNode*) child1)->functionType == FunctionTypeNot)
				parantheses = false;

			if (parantheses)
				printf("(");
			child1->print();
			if (parantheses)
				printf(")");
		}
	}

	virtual bool increment()
	{
		// TODO: Write increment code
		return true;
	}
};

pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

size_t bitsCount;
size_t squareCount;
size_t sideLength;
size_t termsOffset;
vector<vector<bool> > variableValues;
vector<vector<bool> > indexToRowColValues;
vector<vector<bool> > rowColToIndexValues;
vector<ThreadInit> threadInit;
vector<pthread_t> threadIDs;
vector<size_t> bestApproximationValue;
size_t desiredChildrenCount = 0;

void createValues()
{
	size_t currentPos = 0;
	size_t currentDiag = 0;
	size_t currentRow = 0;
	size_t currentCol = 0;

	for (size_t currentPos=0; currentPos<squareCount; currentPos++)
	{
		indexToRowColValues.push_back(vector<bool>());
		rowColToIndexValues.push_back(vector<bool>());
		variableValues.push_back(vector<bool>());
	}

	for (size_t currentPos=0; currentPos<squareCount; currentPos++)
	{
		for (size_t i=0; i<functionsCount; i++)
		{
			variableValues.at(currentPos).push_back(currentPos & (1<<i) ? true : false);
		}

		for (size_t i=0; i<bitsCount; i++)
		{
			indexToRowColValues.at(currentPos).push_back(currentCol & (1<<i) ? true : false);
		}
		for (size_t i=0; i<bitsCount; i++)
		{
			indexToRowColValues.at(currentPos).push_back(currentRow & (1<<i) ? true : false);
		}

		for (size_t i=0; i<functionsCount; i++)
		{
			rowColToIndexValues.at(currentRow*sideLength+currentCol).push_back(currentPos & (1<<i) ? true : false);
		}

		if (currentDiag % 2 == 0) {
			if (currentRow == 0)
			{
				currentCol++;
				currentDiag++;
			}
			else if (currentCol == sideLength-1)
			{
				currentRow++;
				currentDiag++;
			}
			else {
				currentCol++;
				currentRow--;
			}
		}
		else {
			if (currentRow == sideLength-1)
			{
				currentCol++;
				currentDiag++;
			}
			else if (currentCol == 0)
			{
				currentRow++;
				currentDiag++;
			}
			else {
				currentCol--;
				currentRow++;
			}
		}
	}
}

void* workerThread(void* threadid)
{
	size_t currentApproximation = 0;
	ThreadInit* pInit = (ThreadInit*) threadid;

	pthread_mutex_lock(&printMutex);
	pInit->print();
	pthread_mutex_unlock(&printMutex);

	FunctionTreeNode root = FunctionTreeNode(pInit->functionAtRoot, desiredChildrenCount);

	pthread_mutex_lock(&printMutex);
	root.print();
	printf("\n");
	pthread_mutex_unlock(&printMutex);

	return NULL;
}

int main(int argc, char** argv)
{
	if (argc==1)
	{
		fprintf(stderr, "Usage: JpegBoolean bitsCount\n");
		exit(1);
	}

	bitsCount = atoi(argv[1]);
	sideLength = 1<<bitsCount;
	squareCount = sideLength*sideLength;
	functionsCount = 2*bitsCount;
	variablesCount = functionsCount;
	termsOffset = squareCount*functionsCount;

	createValues();

	printf("Index to Row, Col\n\n");

	for (size_t currentPos=0; currentPos<squareCount; currentPos++)
	{
		for (size_t currentFunction=0; currentFunction<functionsCount; currentFunction++)
		{
			printf("%c", variableValues.at(currentPos).at(functionsCount-1-currentFunction) ? '1' : '0');
		}

		printf("\t");

		for (size_t currentFunction=0; currentFunction<functionsCount; currentFunction++)
		{
			printf("%c", indexToRowColValues.at(currentPos).at(functionsCount-1-currentFunction) ? '1' : '0');
		}

		printf("\n");
	}

	printf("\n\n\n\n\nRow, Col to Index\n\n");

	for (size_t currentPos=0; currentPos<squareCount; currentPos++)
	{
		for (size_t currentFunction=0; currentFunction<functionsCount; currentFunction++)
		{
			printf("%c", variableValues.at(currentPos).at(functionsCount-1-currentFunction) ? '1' : '0');
		}

		printf("\t");

		for (size_t currentFunction=0; currentFunction<functionsCount; currentFunction++)
		{
			printf("%c", rowColToIndexValues.at(currentPos).at(functionsCount-1-currentFunction) ? '1' : '0');
		}

		printf("\n");
	}

	printf("\n\n\n\n\n");

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Initialize thread init
	for (size_t useRowColToIndex=0; useRowColToIndex<2; useRowColToIndex++)
	{
		for (size_t functionNumber=0; functionNumber<functionsCount; functionNumber++)
		{
			for (size_t functionAtRoot=0; functionAtRoot<FunctionTypeOr+1; functionAtRoot++)
			{
				ThreadInit init={useRowColToIndex!=0, functionNumber, (FunctionType) functionAtRoot};
				threadInit.push_back(init);
			}
		}
	}

	// 2 variants of each function
	for (size_t i=0; i!=2*functionsCount; i++)
	{
		bestApproximationValue.push_back(0);
	}

	// Begin loop
	threadIDs = vector<pthread_t>();

	for (size_t index=0; index<2*functionsCount; index++)
	{
		if (!bestApproximationValue.at(index)!=squareCount)
		{
			for (size_t currentThreadIndex=0; currentThreadIndex<4; currentThreadIndex++)
			{
				pthread_t id;
				int res = pthread_create(&id, NULL, workerThread, &threadInit.at(4*index+currentThreadIndex));

				if (res!=0)
				{
					fprintf(stderr, "Thread could not be created\n");
					exit(2);
				}

				threadIDs.push_back(id);
			}
		}
	}

	size_t threadIndex = 0;

	for (size_t index=0; index<2*functionsCount; index++)
	{
		if (!bestApproximationValue.at(index)!=squareCount)
		{
			void* status;

			for (size_t currentThreadIndex=0; currentThreadIndex<4; currentThreadIndex++)
			{
				pthread_join(threadIDs.at(threadIndex++), &status);
			}
		}
	}

	desiredChildrenCount++;
	// end loop

	pthread_attr_destroy(&attr);
	return 0;
}

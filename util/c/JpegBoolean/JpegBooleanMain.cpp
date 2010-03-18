#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cassert>
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
	size_t functionNumber;
	FunctionType functionAtRoot;

	void print()
	{
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
	}
};

// the number of variable
size_t varCount;
// the number of functions using these variables
size_t funcCount;

size_t getThreadGroupNumber(ThreadInit threadInit)
{
	return threadInit.functionNumber;
}

size_t getThreadNumber(ThreadInit threadInit)
{
	return 4*getThreadGroupNumber(threadInit)+threadInit.functionAtRoot;
}

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
		if (variableNumber+1<varCount)
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
vector<vector<bool> > variableValues;
size_t variableValuesCount;
// at the moment: 
// first half: index to row, col values
// second half: row, col values to index
vector<vector<bool> > functionValues;
vector<ThreadInit> threadInit;
vector<pthread_t> threadIDs;

// per thread group (4 threads per thread group)
vector<bool> isThreadGroupFinished;
vector<size_t> bestApproximationValues;
vector<pthread_mutex_t> bestApproximationValueMutexes;

size_t desiredChildrenCount;

void createValues()
{
	size_t currentPos = 0;
	size_t currentDiag = 0;
	size_t currentRow = 0;
	size_t currentCol = 0;

	vector<vector<bool> > indexToRowColValues, rowColToIndexValues;

	for (size_t currentPos=0; currentPos<variableValuesCount; currentPos++)
	{
		variableValues.push_back(vector<bool>());
		functionValues.push_back(vector<bool>());
		indexToRowColValues.push_back(vector<bool>());
		rowColToIndexValues.push_back(vector<bool>());
	}

	for (size_t currentPos=0; currentPos<variableValuesCount; currentPos++)
	{
		for (size_t i=0; i<varCount; i++)
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

		for (size_t i=0; i<varCount; i++)
		{
			rowColToIndexValues.at(currentRow*(1<<bitsCount)+currentCol).push_back(currentPos & (1<<i) ? true : false);
		}

		if (currentDiag % 2 == 0) {
			if (currentRow == 0)
			{
				currentCol++;
				currentDiag++;
			}
			else if (currentCol == (1<<bitsCount)-1)
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
			if (currentRow == (1<<bitsCount)-1)
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

	for (size_t currentPos=0; currentPos<variableValuesCount; currentPos++)
	{
		functionValues.at(currentPos).insert(functionValues.at(currentPos).end(), 
			indexToRowColValues.at(currentPos).begin(), 
			indexToRowColValues.at(currentPos).end());
		functionValues.at(currentPos).insert(functionValues.at(currentPos).end(), 
			rowColToIndexValues.at(currentPos).begin(), 
			rowColToIndexValues.at(currentPos).end());
	}
}

void* workerThread(void* threadid)
{
	size_t currentApproximation = 0;
	ThreadInit* pInit = (ThreadInit*) threadid;

	pthread_mutex_lock(&printMutex);
	printf("Starting ");
	pInit->print();
	printf("\n");
	pthread_mutex_unlock(&printMutex);

	size_t threadGroupNumber = getThreadGroupNumber(*pInit);
	size_t threadNumber = getThreadNumber(*pInit);

	FunctionTreeNode root = FunctionTreeNode(pInit->functionAtRoot, desiredChildrenCount);

	// Begin loop
	size_t currentApproximationQuality = 0;

	for (size_t currentVarIdx=0; currentVarIdx<variableValuesCount; currentVarIdx++)
	{
		bool val = root.computeValue(&variableValues.at(currentVarIdx));
		if (val == functionValues.at(currentVarIdx).at(pInit->functionNumber))
			currentApproximationQuality++;
	}

	pthread_mutex_lock(&bestApproximationValueMutexes.at(threadGroupNumber));
	size_t approximationQuality = bestApproximationValues.at(threadGroupNumber);
	if (currentApproximationQuality>approximationQuality)
	{
		bestApproximationValues.at(threadGroupNumber) = currentApproximationQuality;
		pthread_mutex_lock(&printMutex);
		printf("Better Approximation by ");
		pInit->print();
		printf("\tApproximation quality: %u with\n", currentApproximationQuality);
		root.print();
		printf("\n");
		pthread_mutex_unlock(&printMutex);
	}
	pthread_mutex_unlock(&bestApproximationValueMutexes.at(threadGroupNumber));
	if (approximationQuality == variableValuesCount)
	{
		pthread_exit(NULL);
	}

	// end loop

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
	varCount = 2*bitsCount;
	variableValuesCount = 1<<varCount;
	funcCount = 4*bitsCount;

	createValues();

	for (size_t currentPos=0; currentPos<variableValuesCount; currentPos++)
	{
		for (size_t currentVar=0; currentVar<varCount; currentVar++)
		{
			printf("%c", variableValues.at(currentPos).at(varCount-1-currentVar) ? '1' : '0');
		}

		printf("\t");

		for (size_t currentFunction=0; currentFunction<funcCount/2; currentFunction++)
		{
			printf("%c", functionValues.at(currentPos).at(funcCount/2-1-currentFunction) ? '1' : '0');
		}

		printf("\t");

		for (size_t currentFunction=0; currentFunction<funcCount/2; currentFunction++)
		{
			printf("%c", functionValues.at(currentPos).at(funcCount-1-currentFunction) ? '1' : '0');
		}

		printf("\n");
	}

	printf("\n\n\n\n\n");

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Initialize thread init
	for (size_t functionNumber=0; functionNumber<funcCount; functionNumber++)
	{
		for (size_t functionAtRoot=0; functionAtRoot<FunctionTypeOr+1; functionAtRoot++)
		{
			ThreadInit init={functionNumber, (FunctionType) functionAtRoot};
			threadInit.push_back(init);
		}
	}

	// Initialize thread groups
	// 2 variants of each function - that is where 2*functionsCount comes from
	for (size_t i=0; i!=funcCount; i++)
	{
		isThreadGroupFinished.push_back(false);
		bestApproximationValues.push_back(0);
		bestApproximationValueMutexes.push_back(PTHREAD_MUTEX_INITIALIZER);
	}

	desiredChildrenCount = 0;

	// Begin loop
	threadIDs = vector<pthread_t>();

	for (size_t index=0; index<funcCount; index++)
	{
		if (!isThreadGroupFinished.at(index))
		{
			for (size_t currentThreadIndex=0; currentThreadIndex<4; currentThreadIndex++)
			{
				pthread_t id;

				assert(index == getThreadGroupNumber(threadInit.at(4*index+currentThreadIndex)));
				int res = pthread_create(&id, NULL, workerThread, &threadInit.at(4*index+currentThreadIndex));

				if (res!=0)
				{
					fprintf(stderr, "Error: Thread could not be created\n");
					exit(2);
				}

				threadIDs.push_back(id);
			}
		}
	}

	size_t threadIndex = 0;

	for (size_t index=0; index<funcCount; index++)
	{
		if (!isThreadGroupFinished.at(index))
		{
			void* status;

			for (size_t currentThreadIndex=0; currentThreadIndex<4; currentThreadIndex++)
			{
				pthread_join(threadIDs.at(threadIndex++), &status);
			}
		}

		// we would not need to lock this mutex since only this thread can access
		// it at the current time - but it does not mind...
		pthread_mutex_lock(&bestApproximationValueMutexes.at(index));
		if (bestApproximationValues.at(index)==variableValuesCount)
		{
			isThreadGroupFinished.at(index) = true;
			pthread_mutex_lock(&printMutex);
			printf("Thread group %u finished.\n", index);
			pthread_mutex_unlock(&printMutex);
		}
		pthread_mutex_unlock(&bestApproximationValueMutexes.at(index));
	}

	desiredChildrenCount++;
	// end loop

	pthread_attr_destroy(&attr);
	return 0;
}

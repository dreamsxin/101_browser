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
};

pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

size_t bitsCount;
size_t sideLength;
size_t squareCount;
// number of functions for one of en- or decoding
size_t functionsCount;
size_t termsOffset;
vector<vector<bool> > variableValues;
vector<vector<bool> > indexToRowColValues;
vector<vector<bool> > rowColToIndexValues;
vector<ThreadInit> threadInit;
vector<pthread_t> threadIDs;
size_t bestApproximation = 0;

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
	printf("%s\t", pInit->rowColToIndex ? "RowCol to Index" : "Index to RowCol");
	printf("Function: %u\tRoot: ", pInit->functionNumber);
	switch (pInit->functionAtRoot)
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

	for (vector<ThreadInit>::iterator i=threadInit.begin(); i!=threadInit.end(); i++)
	{
		pthread_t id;
		int res = pthread_create(&id, NULL, workerThread, &(*i));
		threadIDs.push_back(id);
	}

	void* status;

	for (vector<pthread_t>::iterator i=threadIDs.begin(); i!=threadIDs.end(); i++)
	{
		pthread_join((*i), &status);
	}

	pthread_attr_destroy(&attr);
	return 0;
}

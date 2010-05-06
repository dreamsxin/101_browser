#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <pthread.h>
#include "Config.h"
#include "BooleanTree.h"
using namespace std;

// the number of variables
size_t varCount;
// the number of functions using these variables
size_t funcCount;

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
	printf(" with children count %u\n", desiredChildrenCount);
	fflush(stdout);
	pthread_mutex_unlock(&printMutex);

	size_t threadGroupNumber = getThreadGroupNumber(*pInit);
	size_t threadNumber = getThreadNumber(*pInit);

	FunctionTreeNode root = FunctionTreeNode(pInit->functionAtEnd, desiredChildrenCount);

	while (true)
	{
		size_t currentApproximationQuality = 0;

		for (size_t currentVarIdx=0; currentVarIdx<variableValuesCount; currentVarIdx++)
		{
			bool val = root.computeValue(&variableValues.at(currentVarIdx), NULL);
			if (val != functionValues.at(currentVarIdx).at(pInit->functionNumber))
				break;
			else
				currentApproximationQuality++;
		}

		pthread_mutex_lock(&bestApproximationValueMutexes.at(threadGroupNumber));
		size_t approximationQuality = bestApproximationValues.at(threadGroupNumber);

		if (currentApproximationQuality>approximationQuality)
		{
			bestApproximationValues.at(threadGroupNumber) = currentApproximationQuality;
			pthread_mutex_unlock(&bestApproximationValueMutexes.at(threadGroupNumber));

			pthread_mutex_lock(&printMutex);
			{
				printf("Better Approximation by ");
				pInit->print();
				printf("\tApproximation quality: %u with\n", currentApproximationQuality);
				root.print();
				printf("\n");
				fflush(stdout);
			}
			pthread_mutex_unlock(&printMutex);
		}
		else
		{
			pthread_mutex_unlock(&bestApproximationValueMutexes.at(threadGroupNumber));
		}

		if (approximationQuality == variableValuesCount)
		{
			pthread_exit(NULL);
		}

		if (root.increment(true))
			continue;
		else
			pthread_exit(NULL);
	}

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
	// 2: since we have both row AND col with bitsCount bits
	varCount = 2*bitsCount;
	variableValuesCount = 1<<varCount;
	// 4 comes from
	// 2: since we have both row AND col with bitsCount bits
	// *
	// 2: for both directions ((row, col) -> index, index -> (row, col))
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
		for (size_t functionAtEnd=0; functionAtEnd<FunctionTypeOr+1; functionAtEnd++)
		{
			ThreadInit init={functionNumber, (FunctionType) functionAtEnd};
			threadInit.push_back(init);
		}
	}

	// Initialize thread groups
	for (size_t i=0; i<funcCount; i++)
	{
		isThreadGroupFinished.push_back(false);
		bestApproximationValues.push_back(0);
		pthread_mutex_t mtx;
		bestApproximationValueMutexes.push_back(mtx);
		pthread_mutex_init(
			&bestApproximationValueMutexes.at(bestApproximationValueMutexes.size()-1), 
			NULL);
	}

	desiredChildrenCount = 0;

	// Begin hack:
	if (bitsCount == 2)
	{
		desiredChildrenCount = 6;
		isThreadGroupFinished.at(1) = true;
		isThreadGroupFinished.at(7) = true;
	}
	else if (bitsCount == 3)
	{
		desiredChildrenCount = 4;
	}
	// End hack

	while (true)
	{
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
				pthread_mutex_unlock(&bestApproximationValueMutexes.at(index));
				isThreadGroupFinished.at(index) = true;
				pthread_mutex_lock(&printMutex);
				printf("Thread group %u finished.\n", index);
				fflush(stdout);
				pthread_mutex_unlock(&printMutex);
			}
			else
			{
				pthread_mutex_unlock(&bestApproximationValueMutexes.at(index));
			}
		}

		bool allThreadsFinished = true;

		for (size_t index=0; index<funcCount; index++)
		{
			if (!isThreadGroupFinished.at(index))
			{
				allThreadsFinished = false;
			}
		}

		if (!allThreadsFinished)
			desiredChildrenCount++;
		else
			break;
	}

	for (size_t i=0; i<funcCount; i++)
	{
		pthread_mutex_destroy(&bestApproximationValueMutexes.at(i));
	}

	pthread_attr_destroy(&attr);
	return 0;
}

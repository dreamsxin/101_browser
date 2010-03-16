#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

size_t bitsCount;
size_t sideLength;
size_t squareCount;
// number of functions for one of en- or decoding
size_t functionsCount;
size_t termsOffset;
vector<vector<bool> > variableValues;
vector<vector<bool> > indexToRowColValues;
vector<vector<bool> > rowColToIndexValues;

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
			indexToRowColValues.at(currentPos).push_back(currentRow & (1<<i) ? true : false);
		}
		for (size_t i=0; i<bitsCount; i++)
		{
			indexToRowColValues.at(currentPos).push_back(currentCol & (1<<i) ? true : false);
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

		for (size_t currentFunction=0; currentFunction<bitsCount; currentFunction++)
		{
			printf("%c", indexToRowColValues.at(currentPos).at(bitsCount-1-currentFunction) ? '1' : '0');
		}
		for (size_t currentFunction=0; currentFunction<bitsCount; currentFunction++)
		{
			printf("%c", indexToRowColValues.at(currentPos).at(2*bitsCount-1-currentFunction) ? '1' : '0');
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
}

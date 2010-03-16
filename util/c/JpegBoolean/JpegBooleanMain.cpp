#include <cstdio>
#include <cstdlib>
#include <cstring>

size_t bitsCount;
size_t squareCount;
// number of functions for one of en- or decoding
size_t functionsCount;
size_t termsOffset;
bool* variableValues=NULL;
bool* decoderValues=NULL;
bool* encoderValues=NULL;

void createValues()
{
	size_t currentPos = 0;
	size_t currentDiag = 0;
	size_t currentRow = 0;
	size_t currentCol = 0;

	for (size_t currentPos=0; currentPos<squareCount; currentPos++)
	{
		size_t pos = currentPos*functionsCount;
		for (size_t i=0; i<functionsCount; i++)
		{
			variableValues[pos++] = currentPos & (1<<i) ? true : false;
		}
		for (size_t i=0; i<bitsCount; i++)
		{
			encoderValues[pos++] = currentRow & (1<<i) ? true : false;
		}
		for (size_t i=0; i<bitsCount; i++)
		{
			encoderValues[pos++] = currentCol & (1<<i) ? true : false;
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

	squareCount = (1<<bitsCount)*(1<<bitsCount);
	functionsCount = 2*bitsCount;
	termsOffset = squareCount*functionsCount;

	variableValues = (bool*) malloc(sizeof(bool)*functionsCount*termsOffset);
	decoderValues = (bool*) malloc(sizeof(bool)*functionsCount*termsOffset);
	encoderValues = (bool*) malloc(sizeof(bool)*functionsCount*termsOffset);

	createValues();

	for (size_t currentPos=0; currentPos<squareCount; currentPos++)
	{
		size_t pos = currentPos*functionsCount;

		for (size_t currentFunction=0; currentFunction<functionsCount; currentFunction++)
		{
			printf("%c", variableValues[pos+(functionsCount-1-currentFunction)] ? '1' : '0');
		}

		printf("\n");
	}
}

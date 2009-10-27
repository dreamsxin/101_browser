#include "BasicDataStructures/Stream/FileReadStream.h"

int main(int argc, char** argv)
{
	FileReadStream stream;

	if (!stream.open("testfiles/swf/watch-vfl127661.swf"))
		return 1;

	printf("Hello World");

	return 0;
}

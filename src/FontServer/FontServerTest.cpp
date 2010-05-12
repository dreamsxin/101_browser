#include "FontServer/FontServer.h"
#include <cstdio>

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		fprintf(stderr, "Usage: FontServer fontfile.ttf\n");
		return 0;
	}

	printf("%i\n", readTTF(argv[1]));

	return 0;
}

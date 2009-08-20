#include "font_server/FontServer.h"
#include <cstdio>

int main(int argc, char** argv)
{
	printf("%i\n", readTTF("../../testfiles/fonts/LucidaBrightRegular.ttf"));

	return 0;
}
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "GIF/GIF.h"

void testGIF()
{
	// We have to be sure that 
	test(sizeof(Logical_Screen_Descriptor) == 7);
	test(sizeof(Graphic_Control_Extension) == 5);
}

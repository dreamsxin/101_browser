#include <stdio.h>
#include <windows.h>
#include "compiler_backend/X86Assembly.h"

int main(int argc, char *argv[])
{
	Program prog;
	prog.push_back(moveToEAX(43));
	prog.push_back(ret());
	CompiledProgram cprog(prog);

	int (*ptr)(void) = static_cast<int (*)(void)> (cprog.program());

    printf("return is %d\n", ptr());
    return 0;
}

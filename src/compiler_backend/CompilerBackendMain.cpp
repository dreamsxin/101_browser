#include <stdio.h>
#include <windows.h>
#include "compiler_backend/X86Assembly.h"

int main(int argc, char *argv[])
{
	Program prog;
	//prog.push_back(moveToRegister32(eax, 44));
	prog.push_back(push32(44));
	prog.push_back(pop32(eax));
	prog.push_back(ret());
	CompiledProgram cprog(prog);

	int (*ptr)(void) = static_cast<int (*)(void)> (cprog.program());

    printf("return is %d\n", ptr());
    return 0;
}

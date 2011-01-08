/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <windows.h>
#include "CompilerBackend/X86Assembly.h"

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

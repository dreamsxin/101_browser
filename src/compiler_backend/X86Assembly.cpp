#include "compiler_backend/X86Assembly.h"

AssemblyInstruction moveToEAX(unsigned int in_value)
{
	unsigned char opCode[5]={0xB8};
	memcpy(opCode+1, &in_value, 4);
	return AssemblyInstruction(opCode, opCode+5);
}

AssemblyInstruction moveToEBX(unsigned int in_value)
{
	unsigned char opCode[5]={0xB9};
	memcpy(opCode+1, &in_value, 4);
	return AssemblyInstruction(opCode, opCode+5);
}

AssemblyInstruction ret()
{
	unsigned char opCode[1]={0xC3};
	return AssemblyInstruction(opCode, opCode+1);
}

CompiledProgram::CompiledProgram(Program in_program)
{
	mProgSize=0;

	for (Program::iterator i=in_program.begin(); i!=in_program.end(); i++)
	{
		mProgSize+=i->size();
	}

	mpProg=VirtualAlloc(
		NULL,
		mProgSize,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
		);

	if (mpProg == NULL )
		exit(0);

	size_t progPos=0;

	for (Program::iterator i=in_program.begin(); i!=in_program.end(); i++)
	{
		memcpy((unsigned char*) mpProg+progPos, &(*i)[0], i->size());
		progPos+=i->size();
	}

	DWORD flOldProtect;

	if (!VirtualProtect(mpProg, size(), PAGE_EXECUTE, &flOldProtect))
		exit(0);
}

CompiledProgram::~CompiledProgram()
{
	if (!VirtualFree(program(), 0, MEM_RELEASE))
		exit(0);
	mpProg=NULL;
}

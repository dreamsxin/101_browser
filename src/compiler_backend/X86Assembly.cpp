#include "compiler_backend/X86Assembly.h"

AssemblyInstruction moveToRegister32(Register32 in_register, int in_value)
{
	unsigned char opCode[5]={static_cast<unsigned char>(0xB8+in_register)};
	memcpy(opCode+1, &in_value, 4);
	return AssemblyInstruction(opCode, opCode+sizeof(opCode));
}

AssemblyInstruction pop32(Register32 in_register)
{
	unsigned char opCode[1]={static_cast<unsigned char>(0x58+in_register)};
	return AssemblyInstruction(opCode, opCode+sizeof(opCode));
}

AssemblyInstruction push32(int in_value)
{
	unsigned char opCode[5]={0x68};
	memcpy(opCode+1, &in_value, 4);
	return AssemblyInstruction(opCode, opCode+sizeof(opCode));
}

AssemblyInstruction ret()
{
	unsigned char opCode[1]={0xC3};
	return AssemblyInstruction(opCode, opCode+sizeof(opCode));
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

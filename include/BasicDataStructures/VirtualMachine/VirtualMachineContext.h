#ifndef _VirtualMachineRunningContext
#define _VirtualMachineRunningContext

#include <vector>
#include "BasicDataStructures/VirtualMachine/InstructionContext.h"

struct VirtualMachineContext
{
	size_t programCounter;
	std::vector<Instruction> instructions;
	std::vector<ReversibleContext> reversibleContexts;

protected:
	bool _inReversibleMode;

public:
	inline VirtualMachineContext() : programCounter(0) { }

	void invoke();
	// simply returns if not in reversible mode - so 
	// you have to check this by yourself
	void revoke();

	inline bool isInReversibleMode()
	{
		return _inReversibleMode;
	}
};

#endif

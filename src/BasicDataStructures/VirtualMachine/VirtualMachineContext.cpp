#include "BasicDataStructures/VirtualMachine/VirtualMachineContext.h"


void VirtualMachineContext::invoke()
{
	

	switch (instructions.at(programCounter))
	{
	case Nop:
		programCounter++;
		if (isInReversibleMode() && reversibleContexts.at(programCounter).markers!=NULL)
		{
			// TODO Fix memory leak
			SingleChain<ReversibleMarker>* currentChain = 
				new SingleChain<ReversibleMarker>();
			appendSingleChain(currentChain, &reversibleContexts.at(programCounter).markers);
		}
	}
}
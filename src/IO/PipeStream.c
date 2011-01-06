#include "IO/PipeStream.h"

void initPipeStreamState(PipeStreamState *in_pPipeStreamState)
{
	in_pPipeStreamState->mpCurrentBuffer = NULL;
	in_pPipeStreamState->mpNextBuffer = NULL;
	in_pPipeStreamState->mCurrentBufferSize = 0;
	in_pPipeStreamState->mNextBufferSize = 0;
	// TODO: in_pPipeStreamState->mpMainDescriptor
	// TODO: in_pPipeStreamState->mReaderDescriptor
	// TODO: in_pPipeStreamState->mWriterDescriptor
}

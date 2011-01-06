#ifndef _MTAx_IO_PipeStream_h
#define _MTAx_IO_PipeStream_h

#include "Coroutine/Coroutine.h"

#include "MiniStdlib/cstddef.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"

typedef struct
{
	uint8_t *mpCurrentBuffer;
	uint8_t *mpNextBuffer;
	volatile size_t mCurrentBufferSize;
	volatile size_t mNextBufferSize;
	CoroutineDescriptor *mpMainDescriptor;
	CoroutineDescriptor mWriterDescriptor;
	CoroutineDescriptor mReaderDescriptor;
} PipeStreamState;

void initPipeStreamState(PipeStreamState *in_pPipeStreamState);

#endif

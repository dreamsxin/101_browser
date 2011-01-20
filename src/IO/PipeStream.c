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

#include "IO/PipeStream.h"

bool initPipeStreamState(PipeStreamState *in_pPipeStreamState, 
	bool in_isCurrentStreamWriter, CoroutineDescriptor *in_pOtherCoroutine)
{
	in_pPipeStreamState->mpCurrentBuffer = NULL;
	in_pPipeStreamState->mpNextBuffer = NULL;
	in_pPipeStreamState->mCurrentBufferSize = 0;
	in_pPipeStreamState->mNextBufferSize = 0;

	if (in_isCurrentStreamWriter)
	{
		in_pPipeStreamState->mCurrentStateType = PipeStreamStateTypeWriter;
		if (!convertThreadToCoroutine(in_pPipeStreamState->mpWriterDescriptor))
			return false;
		//in_pPipeStreamState->mpReaderCoroutine = createCoroutine(...)
	}
	else
	{
		in_pPipeStreamState->mCurrentStateType = PipeStreamStateTypeReader;
		if (!convertThreadToCoroutine(in_pPipeStreamState->mpReaderDescriptor))
			return false;
		//in_pPipeStreamState->mpWriterCoroutine = createCoroutine(...)
	}

	return true;
}

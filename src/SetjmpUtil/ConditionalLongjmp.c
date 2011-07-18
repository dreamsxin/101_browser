/*
* Copyright 2011 Wolfgang Keller
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

#include "SetjmpUtil/ConditionalLongjmp.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/MTAx_cstdlib.h" // for malloc

void printHandler(void *in_pString)
{
	fprintf(stderr, "%s\n", (const char *) in_pString);
}

void setjmpStateLongjmpIf(SetjmpState *in_out_pSetjmpState, 
	bool condition, void *in_pLongjmpHandlerParam)
{
	if (condition)
		setjmpStateLongjmp(in_out_pSetjmpState, in_pLongjmpHandlerParam);
}

void* setjmpStateLongjmpMalloc(SetjmpState *in_out_pSetjmpState, 
	size_t in_size)
{
	void *out_p = malloc(in_size);

	if (!out_p)
		setjmpStateLongjmp(in_out_pSetjmpState, NULL);

	return out_p;
}

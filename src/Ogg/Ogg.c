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

#include "Ogg/Ogg.h"
#include "MiniStdlib/cstring.h"

const char OggS[4] = { 'O', 'g', 'g', 'S' };

ReadResult readOgg(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface)
{
	OggPageHeader oggPageHeader;

	if (in_readInterface.mpfRead(in_out_pReadStreamState, &oggPageHeader, 
		sizeof(OggPageHeader)) != sizeof(OggPageHeader))
		return ReadResultPrematureEndOfStream;

	if (!memcmp(&oggPageHeader.capture_pattern, OggS, 4))
		return ReadResultInvalidData;

	if (oggPageHeader.stream_structure_version != 0)
		return ReadResultNotImplemented;

	if (!oggPageHeader.header_type_flag.bos)
		return ReadResultInvalidData;

	return ReadResultOK;
}


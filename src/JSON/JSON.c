/*
* Copyright 2012 Wolfgang Keller
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

#include "JSON/JSON.h"
#include <assert.h> // assert
#include <stddef.h> // size_t

ReadResult parseJSON(ByteStreamReadInterface_v3 in_readInterface, void *in_pReadState)
{
	unsigned char fourBytes[4];
	size_t bytesReadCountForDetectingEncoding;

	if (in_readInterface.mpfRead(in_pReadState, fourBytes, 2) != 2)
		return ReadResultPrematureEndOfStream;

	/*
	* RFC 4627
	*
	* "3.  Encoding
	*
	*    JSON text SHALL be encoded in Unicode.  The default encoding is
	*    UTF-8.
	* 
	*    Since the first two characters of a JSON text will always be ASCII
	*    characters [RFC0020], it is possible to determine whether an octet
	*    stream is UTF-8, UTF-16 (BE or LE), or UTF-32 (BE or LE) by looking
	*    at the pattern of nulls in the first four octets.
	* 
	*            00 00 00 xx  UTF-32BE
	*            00 xx 00 xx  UTF-16BE
	*            xx 00 00 00  UTF-32LE
	*            xx 00 xx 00  UTF-16LE
	*            xx xx xx xx  UTF-8"
	*/
	     if (0x0 != fourBytes[0] && 0x0 != fourBytes[1])
	{
		// UTF-8

		bytesReadCountForDetectingEncoding = 2;
	}
	else if (0x0 == fourBytes[0] && 0x0 != fourBytes[1])
	{
		// UTF-16BE
		
		bytesReadCountForDetectingEncoding = 2;
	}
	else if (0x0 == fourBytes[0] && 0x0 == fourBytes[1])
	{
		// UTF-32BE
		
		bytesReadCountForDetectingEncoding = 2;
	}
	else
	{
		// UTF-32LE or UTF-16LE

		assert(0x0 != fourBytes[0]);
		assert(0x0 == fourBytes[1]);

		if (in_readInterface.mpfRead(in_pReadState, fourBytes+2, 2) != 2)
			return ReadResultPrematureEndOfStream;

		     if (0x0 != fourBytes[2] && 0x0 == fourBytes[3])
		{
			// UTF16-LE
			
			bytesReadCountForDetectingEncoding = 4;
		}
		else if (0x0 == fourBytes[2] && 0x0 == fourBytes[3])
		{
			// UTF32-LE
			
			bytesReadCountForDetectingEncoding = 4;
		}
		else
			return ReadResultInvalidData;
	}

	return ReadResultOK;
}

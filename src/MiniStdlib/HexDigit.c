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

#include "MiniStdlib/HexDigit.h"

bool convertHexDigitToNumber(UnicodeCodePoint token, uint8_t *out_pNumber, 
	bool in_acceptUppercase, bool in_acceptLowercase)
{
	if (token >= '0' && token <= '9')
	{
		*out_pNumber = (token - '0');
		return true;
	}
	else if (token >= 'A' && token <= 'F' && in_acceptUppercase)
	{
		*out_pNumber = 0xA + (token - 'A');
		return true;
	}
	else if (token >= 'a' && token <= 'f' && in_acceptLowercase)
	{
		*out_pNumber = 0xA + (token - 'a');
		return true;
	}
	else
	{
		return false;
	}
}

void convertNumberToHexDigits(uint8_t in_number, bool in_lowercase, char out_characters[2])
{
	uint8_t nibbles[2] = { in_number >> 4, in_number & 0x0F };
	uint8_t idx;

	for (idx = 0; idx < 2; idx++)
	{
		if (nibbles[idx] < 0xA)
			out_characters[idx] = '0' + nibbles[idx];
		else
		{
			nibbles[idx] -= 0xA;
			
			if (in_lowercase)
			{
				out_characters[idx] = 'a' + nibbles[idx];
			}
			else
			{
				out_characters[idx] = 'A' + nibbles[idx];
			}
		}
	}
}
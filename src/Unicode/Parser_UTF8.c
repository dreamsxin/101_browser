/*
* Copyright 2008-2012 Wolfgang Keller
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

#include "Unicode/Parser.h"
#include "Unicode/Parser_Util.h"
#include "Algorithm/Bitmask.h"
#include "MiniStdlib/cstdint.h"
#include <assert.h>

void utf8_StateInit(UTF8_State *out_pState)
{
	utf8_StateReset(out_pState);
}

void utf8_StateReset(UTF8_State *out_pState)
{
	out_pState->entryPoint = UTF8_EntryPoint_Begin;
	out_pState->internalState = UTF8_InternalState_Start;
}

void append(UnicodeCodePoint *in_out_pCodepoint, uint8_t in_currentByte)
{
	*in_out_pCodepoint <<= 6;
	*in_out_pCodepoint |= (in_currentByte & 0x3F);
}

ParseBlocker utf8_parse(
	void *in_out_pParserState, 
	void *in_pReadState, ByteStreamReadInterface_v4 in_readInterface, 
	void *in_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface)
{
	UTF8_State *pUTF8State = (UTF8_State *) in_out_pParserState;
	extern const UnicodeCodePoint cReplacementCharacter;

	switch (pUTF8State->entryPoint)
	{
	case UTF8_EntryPoint_Begin:
		break;
	case UTF8_EntryPoint_WriteTerminalReplacementCharacter:
		goto Label_EntryPoint_WriteTerminalReplacementCharacter;
	case UTF8_EntryPoint_Start_leq_0x7F:
		goto Label_EntryPoint_Start_leq_0x7F;
	case UTF8_EntryPoint_Start_between_0x80_0xBF:
		goto Label_EntryPoint_Start_between_0x80_0xBF;
	case UTF8_EntryPoint_Start_FE_FF:
		goto Label_EntryPoint_Start_FE_FF;
	case UTF8_EntryPoint_not_Start_leq_0x7F_or_geq_0xC0:
		goto Label_EntryPoint_not_Start_leq_0x7F_or_geq_0xC0;
	case UTF8_EntryPoint_X_error_between_0x80_0xBF:
		goto Label_EntryPoint_X_error_between_0x80_0xBF;
	case UTF8_EntryPoint_X_append_emit_between_0x80_0xBF:
		goto Label_EntryPoint_X_append_emit_between_0x80_0xBF;
	case UTF8_EntryPoint_Terminated:
		goto Label_EntryPoint_Terminated;
	}
	
	while (1)
	{
		size_t tempReadCount = in_readInterface.mpfRead(in_pReadState, &pUTF8State->currentByte, 1);
		
		if (0 == tempReadCount)
		{
			ByteStreamStatus_v4 status = in_readInterface.
				commonByteStreamInterface.mpfGetStatus(in_pReadState);
			assert(ByteStreamStatus_OK != status);
			
			pUTF8State->entryPoint = UTF8_EntryPoint_Begin;

			if (ByteStreamStatus_Terminated == status)
			{
				if (UTF8_InternalState_Start != pUTF8State->internalState)
				{
Label_EntryPoint_WriteTerminalReplacementCharacter:
					if (emitCodepoint(in_pWriteState, in_writeInterface, 
						cReplacementCharacter, &pUTF8State->entryPoint, 
						sizeof(UTF8_EntryPoint), 
						UTF8_EntryPoint_WriteTerminalReplacementCharacter))
						return ParseBlocker_Writer;
				}

				goto terminate;
			}
			else
			{
				// Entry point was set above
				return ParseBlocker_Reader;
			}
		}

		assert(1 == tempReadCount);

		if (UTF8_InternalState_Start == pUTF8State->internalState)
		{
Action_of_Start:
			assert(UTF8_InternalState_Start == pUTF8State->internalState);

			if (pUTF8State->currentByte <= 0x7F)
			{
Label_EntryPoint_Start_leq_0x7F:
				if (emitCodepoint(in_pWriteState, in_writeInterface, 
					(UnicodeCodePoint) pUTF8State->currentByte, 
					&pUTF8State->entryPoint, sizeof(UTF8_EntryPoint), 
					UTF8_EntryPoint_Start_leq_0x7F))
					return ParseBlocker_Writer;

				// No need to set state to UTF8_InternalState_Start because of assertion above
			}
			else if (pUTF8State->currentByte <= 0xBF)
			{
Label_EntryPoint_Start_between_0x80_0xBF:
				if (emitCodepoint(in_pWriteState, in_writeInterface, cReplacementCharacter, 
					&pUTF8State->entryPoint, sizeof(UTF8_EntryPoint), 
					UTF8_EntryPoint_Start_between_0x80_0xBF))
					return ParseBlocker_Writer;
				
				// No need to set state to UTF8_InternalState_Start because of assertion above
			}
			else if (0xC0 == pUTF8State->currentByte || 0xC1 == pUTF8State->currentByte)
			{
				pUTF8State->internalState = UTF8_InternalState_X_error;
			}
			else if (pUTF8State->currentByte <= 0xDF)
			{
				pUTF8State->codePoint = pUTF8State->currentByte & 0x1F;
				pUTF8State->internalState = UTF8_InternalState_X_append_emit;
			}
			else if (0xE0 == pUTF8State->currentByte)
			{
				pUTF8State->codePoint = 0;
				pUTF8State->internalState = UTF8_InternalState_E0;
			}
			else if (pUTF8State->currentByte <= 0xEF && pUTF8State->currentByte != 0xED)
			{
				pUTF8State->codePoint = pUTF8State->currentByte & 0x0F;
				pUTF8State->internalState = UTF8_InternalState_X_X_append_emit;
			}
			else if (0xED == pUTF8State->currentByte)
			{
				pUTF8State->codePoint = 0xD;
				pUTF8State->internalState = UTF8_InternalState_ED;
			}
			else if (0xF0 == pUTF8State->currentByte)
			{
				pUTF8State->codePoint = 0;
				pUTF8State->internalState = UTF8_InternalState_F0;
			}
			else if (pUTF8State->currentByte <= 0xF3)
			{
				pUTF8State->codePoint = pUTF8State->currentByte & 0x03;
				pUTF8State->internalState = UTF8_InternalState_X_X_X_append_emit;
			}
			else if (0xF4 == pUTF8State->currentByte)
			{
				pUTF8State->codePoint = 4;
				pUTF8State->internalState = UTF8_InternalState_F4;
			}
			else if (pUTF8State->currentByte <= 0xF7)
			{
				pUTF8State->internalState = UTF8_InternalState_X_X_X_error;
			}
			else if (pUTF8State->currentByte <= 0xFB)
			{
				pUTF8State->internalState = UTF8_InternalState_X_X_X_X_error;
			}
			else if (pUTF8State->currentByte <= 0xFD)
			{
				pUTF8State->internalState = UTF8_InternalState_X_X_X_X_X_error;
			}
			else
			{
				assert(0xFE == pUTF8State->currentByte || 0xFF == pUTF8State->currentByte);

Label_EntryPoint_Start_FE_FF:
				if (emitCodepoint(in_pWriteState, in_writeInterface, cReplacementCharacter, 
					&pUTF8State->entryPoint, sizeof(UTF8_EntryPoint), 
					UTF8_EntryPoint_Start_FE_FF))
					return ParseBlocker_Writer;
				
				// No need to set state to UTF8_InternalState_Start because of assertion above
			}
		}
		else
		{
			if ((pUTF8State->currentByte <= 0x7F) || (0xC0 <= pUTF8State->currentByte))
			{
Label_EntryPoint_not_Start_leq_0x7F_or_geq_0xC0:
				if (emitCodepoint(in_pWriteState, in_writeInterface, cReplacementCharacter, 
					&pUTF8State->entryPoint, sizeof(UTF8_EntryPoint), 
					UTF8_EntryPoint_not_Start_leq_0x7F_or_geq_0xC0))
					return ParseBlocker_Writer;

				pUTF8State->internalState = UTF8_InternalState_Start;
				goto Action_of_Start;
			}
			else
			{
				assert(0x80 <= pUTF8State->currentByte);
				assert(pUTF8State->currentByte <= 0xBF);

				if ((UTF8_InternalState_X_error < pUTF8State->internalState &&
					UTF8_InternalState_X_X_X_X_X_error <= pUTF8State->internalState) ||
					UTF8_InternalState_X_X_append_emit == pUTF8State->internalState ||
					UTF8_InternalState_X_X_X_append_emit == pUTF8State->internalState)
				{
					pUTF8State->internalState = (UTF8_InternalState) 
						((unsigned int) pUTF8State->internalState-1);
				}
				else if (UTF8_InternalState_X_error == pUTF8State->internalState)
				{
Label_EntryPoint_X_error_between_0x80_0xBF:
					if (emitCodepoint(in_pWriteState, in_writeInterface, 
						cReplacementCharacter, 
						&pUTF8State->entryPoint, sizeof(UTF8_EntryPoint), 
						UTF8_EntryPoint_X_error_between_0x80_0xBF))
						return ParseBlocker_Writer;

					pUTF8State->internalState = UTF8_InternalState_Start;
				}
				else if (UTF8_InternalState_X_append_emit == pUTF8State->internalState)
				{
Label_EntryPoint_X_append_emit_between_0x80_0xBF:
					if (emitCodepoint(in_pWriteState, in_writeInterface, pUTF8State->codePoint, 
						&pUTF8State->entryPoint, sizeof(UTF8_EntryPoint), 
						UTF8_EntryPoint_X_append_emit_between_0x80_0xBF))
						return ParseBlocker_Writer;
					
					pUTF8State->internalState = UTF8_InternalState_Start;
				}
				else if (UTF8_InternalState_E0 == pUTF8State->internalState)
				{
					if (pUTF8State->currentByte <= 0x9F)
					{
						pUTF8State->internalState = UTF8_InternalState_X_error;
					}
					else
					{
						assert(pUTF8State->currentByte >= 0xA0);
						assert(pUTF8State->currentByte <= 0xBF);
						
						append(&pUTF8State->codePoint, pUTF8State->currentByte);
						pUTF8State->internalState = UTF8_InternalState_X_append_emit;
					}
				}
				else if (UTF8_InternalState_ED == pUTF8State->internalState)
				{
					if (pUTF8State->currentByte <= 0x9F)
					{
						append(&pUTF8State->codePoint, pUTF8State->currentByte);
						pUTF8State->internalState = UTF8_InternalState_X_append_emit;
					}
					else
					{
						assert(pUTF8State->currentByte >= 0xA0);
						assert(pUTF8State->currentByte <= 0xBF);

						pUTF8State->internalState = UTF8_InternalState_X_error;
					}
				}
				else if (UTF8_InternalState_F0 == pUTF8State->internalState)
				{
					if (pUTF8State->currentByte <= 0x8F)
					{
						pUTF8State->internalState = UTF8_InternalState_X_X_error;
					}
					else
					{
						assert(pUTF8State->currentByte >= 0x90);
						assert(pUTF8State->currentByte <= 0xBF);

						append(&pUTF8State->codePoint, pUTF8State->currentByte);
						pUTF8State->internalState = UTF8_InternalState_X_X_append_emit;
					}
				}
				else if (UTF8_InternalState_F4 == pUTF8State->internalState)
				{
					if (pUTF8State->currentByte <= 0x8F)
					{
						append(&pUTF8State->codePoint, pUTF8State->currentByte);
						pUTF8State->internalState = UTF8_InternalState_X_X_append_emit;
					}
					else
					{
						assert(pUTF8State->currentByte >= 0x90);
						assert(pUTF8State->currentByte <= 0xBF);
						
						pUTF8State->internalState = UTF8_InternalState_X_X_error;
					}
				}
			}
		}
	}

terminate:
	pUTF8State->entryPoint = UTF8_EntryPoint_Terminated;

Label_EntryPoint_Terminated:
	assert(ByteStreamStatus_Terminated == in_readInterface.
		commonByteStreamInterface.mpfGetStatus(in_pReadState));
	return ParseBlocker_Neither;
}

#include "BasicDataStructures/Parser/ParserUnicodeEncodings.h"

Result<unsigned char> isUtf8TokenValid(Utf8Token token)
{
	if (!(token.data[0] & 0x80)) // highest bit of token.data[0] not set
	{
		Result<unsigned char> out_result = {cStatusOK, 1};
		return out_result;
	}
	else if (!(token.data[0] & 0xB)) // highest two bits are 10
	{
		Result<unsigned char> out_result = {cStatusOK, 1};
		return out_result;
	}
	else
	{
		Result<unsigned char> out_result = {cStatusErrorInvalidData};
		return out_result;
	}
}
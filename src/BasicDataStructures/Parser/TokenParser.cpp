#include "BasicDataStructures/Parser/ParserTypes.h"

TokenParser::TokenParser(char in_token) : token(in_token)
{
	initialize();
}

void TokenParser::initialize()
{
	parsed = false;
	valid = true;
}

bool TokenParser::isFinished() const
{
	if (valid)
		return parsed;
	else
		return false;
}

void TokenParser::parseToken(char in_token)
{
	if (!parsed)
	{
		parsed = true;
		valid = (in_token == token);
	}
}

bool TokenParser::acceptWhenTerminate() const
{
	return valid;
}

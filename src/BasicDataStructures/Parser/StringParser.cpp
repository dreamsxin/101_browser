#include "BasicDataStructures/Parser/StringParser.h"

bool StringParser::open(std::string in_stringToParse)
{
	mParsedPartsOK = true;
	mStringToParse = in_stringToParse;
	mCurrentIndex = 0;

	return isOpen();
}

void StringParser::close()
{
	mParsedPartsOK = false;
}

bool StringParser::isOpen() const
{
	return mParsedPartsOK;
}

bool StringParser::parseToken(char in_token)
{
	if (!mParsedPartsOK)
		return false;

	if (mCurrentIndex>=mStringToParse.length())
		return false;

	if (in_token!=mStringToParse.at(mCurrentIndex))
	{
		mParsedPartsOK = false;
		return false;
	}

	return true;
}

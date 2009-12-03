#include "BasicDataStructures/Parser/StringParser.h"


bool StringParser::parseTokenIfNotDone(char in_token)
{
	if (!mParsedPartsOK)
		return false;

	if (in_token!=mStringToParse.at(mCurrentIndex))
	{
		mParsedPartsOK = false;
		return false;
	}

	return true;
}

void StringParser::reset()
{
	mParsedPartsOK = true;
	mCurrentIndex = 0;
}

bool StringParser::isDone() const
{
	return mCurrentIndex == mStringToParse.length();
}

bool StringParser::isOK() const
{
	return mParsedPartsOK;
}

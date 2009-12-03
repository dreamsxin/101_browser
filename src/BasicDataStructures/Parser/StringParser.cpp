#include "BasicDataStructures/Parser/StringParser.h"

bool StringParser::parseTokenIfNotDone(char in_token)
{
	if (in_token!=mStringToParse.at(mCurrentIndex))
	{
		mParsedPartsOK = false;
	}

	return mParsedPartsOK;
}

void StringParser::reset()
{
	mParsedPartsOK = true;
	mCurrentIndex = 0;
}

bool StringParser::isDone() const
{
	return mCurrentIndex == mStringToParse.length() || !mParsedPartsOK;
}

bool StringParser::isOK() const
{
	return mCurrentIndex == mStringToParse.length() && mParsedPartsOK;
}

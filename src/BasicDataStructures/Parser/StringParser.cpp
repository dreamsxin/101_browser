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

size_t StringParser::write(const void* in_buffer, size_t in_size, size_t in_count)
{
	if (in_size==0)
	{
		if (mParsedPartsOK)
			return in_count;
		else
			return 0;
	}

	size_t parsedCount = 0;

	for (size_t i=0; i<in_count; i++)
	{
		if (!mParsedPartsOK)
			break;

		if (mCurrentIndex>=mStringToParse.length())
			break;

		if (((unsigned char*) in_buffer)[in_size*parsedCount]!=mStringToParse.at(mCurrentIndex))
		{
			mParsedPartsOK = false;
			break;
		}

		mCurrentIndex++;
		parsedCount++;
	}

	return parsedCount;
}

#include "BasicDataStructures/Parser/UntilCharacterParser.h"

bool UntilCharacterParser::open(char in_untilCharacter)
{
	mReachedCharacter = false;
	mUntilCharacter = in_untilCharacter;
	mReadCharacters=std::list<char>();
	return isOpen();
}

bool UntilCharacterParser::isOpen() const
{
	return !mReachedCharacter;
}

void UntilCharacterParser::close()
{
	mReachedCharacter = true;
}

size_t UntilCharacterParser::write(const void* in_buffer, size_t in_size, size_t in_count)
{
	if (in_size==0)
	{
		if (!mReachedCharacter)
			return in_count;
		else
			return 0;
	}

	size_t parsedCount = 0;

	for (size_t i=0; i<in_count; i++)
	{
		if (mReachedCharacter)
			break;

		if (((unsigned char*) in_buffer)[in_size*parsedCount]!=mUntilCharacter)
		{
			mReadCharacters.push_back(((unsigned char*) in_buffer)[in_size*parsedCount]);
			parsedCount++;
		}
		else
		{
			mReachedCharacter = true;
			break;
		}
	}

	return parsedCount;
}

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

bool UntilCharacterParser::parseToken(char in_token)
{
	if (mReachedCharacter)
		return false;

	if (in_token!=mUntilCharacter)
	{
		mReadCharacters.push_back(in_token);
	}
	else
	{
		mReachedCharacter = true;
	}

	return true;
}

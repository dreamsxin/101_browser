#include "BasicDataStructures/Parser/UntilCharacterParser.h"

bool UntilCharacterParser::parseTokenIfNotDone(char in_token)
{
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

void UntilCharacterParser::reset()
{
	mReachedCharacter = false;
	mReadCharacters=std::list<char>();
}

bool UntilCharacterParser::isDone() const
{
	return mReachedCharacter;
}

bool UntilCharacterParser::isOK() const
{
	return mReachedCharacter;
}

#ifndef _UntilCharacterParser_h
#define _UntilCharacterParser_h

#include "BasicDataStructures/Parser/AbstractParser.h"
#include <list>

class UntilCharacterParser : public AbstractParser
{
protected:
	bool mReachedCharacter;
	char mUntilCharacter;

	virtual bool parseTokenIfNotDone(char in_token);

public:
	std::list<char> mReadCharacters;

	inline UntilCharacterParser(char in_untilCharacter) : mUntilCharacter(in_untilCharacter)
	{
		reset();
	}

	virtual void reset();
	virtual bool isDone() const;
	virtual bool isOK() const;
};

#endif

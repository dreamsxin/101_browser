#ifndef _UntilCharacterParser_h
#define _UntilCharacterParser_h

#include "BasicDataStructures/Parser/AbstractParser.h"
#include <list>

class UntilCharacterParser : public AbstractParser<char>
{
protected:
	bool mReachedCharacter;
	char mUntilCharacter;	

public:
	std::list<char> mReadCharacters;

	inline UntilCharacterParser()
	{
		open(0x0);
	}

	virtual bool open(char in_untilCharacter);
	virtual bool isOpen() const;
	virtual void close();

	virtual bool parseToken(char in_token);
};

#endif

#ifndef _StringParser_h
#define _StringParser_h

#include "BasicDataStructures/Parser/AbstractParser.h"
#include <string>

class StringParser : public AbstractParser
{
protected:
	bool mParsedPartsOK;
	std::string mStringToParse;
	size_t mCurrentIndex;

	virtual bool parseTokenIfNotDone(char in_token);

public:
	inline StringParser(std::string in_string_to_parse)
		: mStringToParse(in_string_to_parse)
	{
		reset();
	}
	
	virtual void reset();
	virtual bool isDone() const;
	virtual bool isOK() const;
};

#endif

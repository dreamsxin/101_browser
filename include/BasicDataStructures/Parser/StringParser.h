#ifndef _StringParser_h
#define _StringParser_h

#include "BasicDataStructures/Parser/AbstractParser.h"
#include <string>

class StringParser : public AbstractParser<std::string>
{
protected:
	bool mParsedPartsOK;
	std::string mStringToParse;
	size_t mCurrentIndex;

public:
	inline StringParser()
	{
		open("");
	}

	virtual bool open(std::string in_stringToParse);
	virtual void close();
	virtual bool isOpen() const;

	virtual size_t write(const void* in_buffer, size_t in_size, size_t in_count);
};

#endif

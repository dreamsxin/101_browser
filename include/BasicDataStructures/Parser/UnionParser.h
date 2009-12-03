#ifndef _UnionParser_h
#define _UnionParser_h

#include "BasicDataStructures/Parser/AbstractParser.h"
#include <list>

class UnionParser : public AbstractParser
{
protected:
	std::list<AbstractParser*> mParsers;
	std::list<AbstractParser*> mNotDoneParsers;

	virtual bool parseTokenIfNotDone(char in_token);

public:
	inline UnionParser(std::list<AbstractParser*> in_parsers) : mParsers(in_parsers)
	{
		reset();
	}

	virtual void reset();
	virtual bool isDone() const;
	virtual bool isOK() const;
};

#endif

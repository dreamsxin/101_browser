#ifndef _ParserTypes_h
#define _ParserTypes_h

#include <string>
#include <list>
#include "BasicDataStructures/Parser/AbstractParser.h"

class TokenParser : public AbstractParser
{
	char token;
	bool parsed; // tells whether the token was already parsed
	bool valid;

public:
	TokenParser(char in_token);

	virtual void initialize();
	virtual bool isFinished() const;
	virtual void parseToken(char in_token);
	virtual bool acceptWhenTerminate() const;
};

class CombineWhenFinishedParser : public AbstractParser
{
	std::list<AbstractParser*> parserList;
	std::list<AbstractParser*>::iterator actParser;

	void increment();

public:
	CombineWhenFinishedParser();

	void addParser(AbstractParser* in_parser);

	virtual void initialize();
	virtual bool isFinished() const;
	virtual void parseToken(char in_token);
	virtual bool acceptWhenTerminate() const;
};

#endif

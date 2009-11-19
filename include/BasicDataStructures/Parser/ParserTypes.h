#ifndef _ParserTypes_h
#define _ParserTypes_h

#include <string>
#include <list>
#include "BasicDataStructures/Parser/AbstractParser.h"

class StringParser : public AbstractParser<std::string>
{

};

/*
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
};*/

#endif

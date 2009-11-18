#include "BasicDataStructures/Parser/ParserTypes.h"

CombineWhenFinishedParser::CombineWhenFinishedParser()
{
	initialize();
}

void CombineWhenFinishedParser::increment()
{
	while (actParser!=parserList.end() && (*actParser)->isFinished())
		actParser++;
}

void CombineWhenFinishedParser::addParser(AbstractParser* in_parser)
{
	parserList.push_back(in_parser);
}

void CombineWhenFinishedParser::initialize()
{
	actParser = parserList.begin();

	increment();

	for (std::list<AbstractParser*>::iterator currentParser = parserList.begin();
		currentParser != parserList.end(); currentParser++)
	{
		(*currentParser)->initialize();
	}
}

bool CombineWhenFinishedParser::isFinished() const
{
	return (actParser == parserList.end());
}

void CombineWhenFinishedParser::parseToken(char in_token)
{
	if (actParser!=parserList.end())
		(*actParser)->parseToken(in_token);

	increment();
}

bool CombineWhenFinishedParser::acceptWhenTerminate() const
{
	if (actParser!=parserList.end())
		return (*actParser)->acceptWhenTerminate();

	return false;
}

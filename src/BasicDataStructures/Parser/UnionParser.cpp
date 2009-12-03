#include "BasicDataStructures/Parser/UnionParser.h"

using namespace std;

bool UnionParser::parseTokenIfNotDone(char in_token)
{
	bool success = false;

	for (list<AbstractParser*>::iterator i=mNotDoneParsers.begin(); 
		i!=mNotDoneParsers.end(); )
	{
		success |= (*i)->parseToken(in_token);

		if ((*i)->isDone())
		{
			mNotDoneParsers.erase(i);
		}
		else
		{
			i++;
		}
	}

	return success;
}

void UnionParser::reset()
{
	mNotDoneParsers = list<AbstractParser*>();
	
	for (list<AbstractParser*>::iterator i=mParsers.begin(); i!=mParsers.end(); i++)
	{
		(*i)->reset();

		if (!(*i)->isDone())
			mNotDoneParsers.push_back(*i);
	}
}

bool UnionParser::isDone() const
{
	return mNotDoneParsers.empty();
}

bool UnionParser::isOK() const
{
	for (list<AbstractParser*>::const_iterator i=mParsers.begin(); i!=mParsers.end(); i++)
	{
		if (!(*i)->isOK())
			return true;
	}

	return false;
}

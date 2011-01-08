/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

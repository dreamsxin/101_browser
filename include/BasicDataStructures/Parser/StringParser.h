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

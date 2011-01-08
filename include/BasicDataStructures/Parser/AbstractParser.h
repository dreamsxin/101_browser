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

#ifndef _AbstractParser_h
#define _AbstractParser_h

#include "BasicDataStructures/Stream/BasicOpenStream.h"
#include "BasicDataStructures/Stream/BasicWriteStream.h"

class AbstractParser
{
protected:
	/** 
	 * parseToken checks whether isDone() returns true.
	 * If this is not the case this function gets called.
	 */
	virtual bool parseTokenIfNotDone(char in_token) = 0;

public:
	/**
	 * Return value: 
	 * true: token was accepted
	 * false: token was not accepted (either parsing finished or error occured)
	 */
	inline virtual bool parseToken(char in_token)
	{
		if (isDone())
			return false;
		else
			return parseTokenIfNotDone(in_token);
	}

	/**
	 * Resets the parser
	 */
	virtual void reset() = 0;

	/**
	 * Returns true if the parser won't accept any more tokens
	 */
	virtual bool isDone() const = 0;

	/**
	 * Returns true, if we could terminate parsing and get a valid result
	 */
	virtual bool isOK() const = 0;
};

#endif

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

#include "BasicDataStructures/Parser/StringParser.h"

bool StringParser::parseTokenIfNotDone(char in_token)
{
	if (in_token!=mStringToParse.at(mCurrentIndex))
	{
		mParsedPartsOK = false;
	}

	return mParsedPartsOK;
}

void StringParser::reset()
{
	mParsedPartsOK = true;
	mCurrentIndex = 0;
}

bool StringParser::isDone() const
{
	return mCurrentIndex == mStringToParse.length() || !mParsedPartsOK;
}

bool StringParser::isOK() const
{
	return mCurrentIndex == mStringToParse.length() && mParsedPartsOK;
}

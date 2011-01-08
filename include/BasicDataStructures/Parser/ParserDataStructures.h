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

#ifndef _ParserDataStructures_h
#define _ParserDataStructures_h

struct ReadByte
{
	bool endOfStream;    // if true, the stream has finished and the value of token is
	                     // undefined
	// here some padding bytes might occur
	unsigned char token; // if endOfStream==false the value of the token is stored here
	// here some padding bytes might occur
};

const unsigned char cStatusOK = 0;
const unsigned char cStatusErrorInvalidData = 1; // we got invalid data - so we could not handle it

template<typename T> struct Result
{
	unsigned char status;
	T result;             // this is the result; if status!=0 its value is not defined
};

#endif

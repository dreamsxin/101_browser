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

#ifndef _Maybe_h
#define _Maybe_h

template <typename T> struct Maybe
{
	bool valid;
	T data; // is only defined if valid == true

	inline Maybe<T>() : valid(false) { }

	inline Maybe<T>(T in_data) : valid(true), data(in_data) { }
};

#endif

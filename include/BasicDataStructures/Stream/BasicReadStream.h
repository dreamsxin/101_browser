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

#ifndef _BasicReadStream_h
#define _BasicReadStream_h

/*!
 * Implements a stream which can be read
 */
class BasicReadStream
{
public:
	virtual ~BasicReadStream() { }

	/*!
	 * Return value:
	 * the number of read structures of size in_size
	 */
	virtual size_t read(void* in_buffer, size_t in_size, size_t in_count) = 0;
};

#endif

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

#ifndef _BasicWriteStream_h
#define _BasicWriteStream_h

class BasicWriteStream
{
	/*!
	 * Return value:
	 * the number of written bytes
	 */
	virtual size_t write(const void* in_buffer, size_t in_size, size_t in_count) = 0;
};

#endif
